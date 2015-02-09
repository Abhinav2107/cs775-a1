#include "bvh.hpp"
#include "error.hpp"

using namespace bvh;

//--------------------------------------------------------------------------------
// bvh_t methods

bvh_t::bvh_t(std::string _filename)
{
  hierarchy = NULL;
  motion = NULL;
  has_motion = true;
  bvh_filename = _filename;
  try { read(); }
  catch (util::common::error *e)
    { util::common::error::propogate_error(e); }
}
 
bvh_t::bvh_t(std::string _filename, bool _has_motion)
{
  hierarchy = NULL;
  motion = NULL;
  has_motion = _has_motion;
  bvh_filename = _filename;
  try { read(); }
  catch (util::common::error *e)
    { util::common::error::propogate_error(e); }
}

bvh_t::~bvh_t()
{
  if (hierarchy != NULL) delete hierarchy;
  if (motion != NULL) delete motion;
}

hierarchy_t* bvh_t::get_hierarchy(void)
{
  return hierarchy;
}

motion_t* bvh_t::get_motion(void)
{
  return motion;
}

std::string bvh_t::get_filename(void)
{
  return bvh_filename;
}

bool bvh_t::get_has_motion(void)
{
  return has_motion;
}

void bvh_t::set_hierarchy(hierarchy_t* _hierarchy)
{
  hierarchy = _hierarchy;
}
	
void bvh_t::set_motion(motion_t* _motion)
{
  motion = _motion;
}

void bvh_t::read(void)
{
  if (bvh_filename.empty())
    throw new util::common::warning_error("(bvh_t::read) : Invalid filename");

  std::ifstream inp (bvh_filename.c_str());
  if (inp == NULL)
    throw new util::common::warning_error("(bvh_t::read) : Unable to open file for reading"); 

  hierarchy = new hierarchy_t;

  try { hierarchy->read(inp); }
  catch (util::common::error *e)
    { util::common::error::propogate_error(e); }

  if (has_motion)
    {
      motion = new motion_t;
      motion->set_data_channels(hierarchy->get_tot_num_channels());
      try { motion->read(inp); }
      catch (util::common::error *e)
	{ util::common::error::propogate_error(e); }
    }
  
  inp.close();
}

void bvh_t::print_hierarchy(std::ostream &out)
{
  try 
    { 
      hierarchy->print_lin(out); 
      out<<"Total number of channels : "<<hierarchy->get_tot_num_channels()<<std::endl;
    }
  catch (util::common::error *e)
    { util::common::error::propogate_error(e); }
}

void bvh_t::print_motion(std::ostream &out)
{
  if (has_motion)
    { 
      motion->print(out); 
    }
  else 
    throw new util::common::warning_error("(bvh_t::print_motion) No motion data available");
}

void bvh_t::render_pose(joint_t *jtptr)
{
  /* CS775: Implement this method */

    // Apply Matrix
    float m[16];
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            m[4*i + j] = jtptr->get_M()[j][i];
        }
    }
    glMultMatrixf(m);


    // Render Joint
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glutSolidSphere(jtptr->get_render_joint_size() * 3, 30, 30);

    // For each child: render link, render joint
    std::list<joint_t *>::const_iterator iterator;
    for(iterator = (*(jtptr->get_childlist())).begin(); iterator != (*(jtptr->get_childlist())).end(); iterator++) {
        joint_t * j = *iterator;
        glPushMatrix();
        util::math::vec3 offset = j->get_offset();
        double len = offset.length();
        if((offset[1] >= 0 && offset[1] < 0.01) && (offset[2] >= 0 && offset[2] < 0.01) && offset[0] < 0)
            glRotatef(180, 0, 1, 0);
        else
            glRotatef(180 * acos(offset[0]/len) / PI, 0, -(len * offset[2]), len * offset[1]);
        glTranslatef(len / 2, 0, 0);
        glScalef(len, 3, 3);
        glColor4f(0.0, 0.0, 1.0, 1.0);
        glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(offset[0], offset[1], offset[2]);
        render_pose(j);
        glPopMatrix();
    }
}

void bvh_t::render_canonical_pose(void)
{
  /* CS775: Implement this method */
    std::list<joint_t *>::const_iterator iterator;
    float data_channels[6] = {};
    for(iterator = (*(hierarchy->get_joint_list())).begin(); iterator != (*(hierarchy->get_joint_list())).end(); iterator++) {
        joint_t * j = *iterator;
        j->update_matrix(data_channels);
    }
    render_pose(hierarchy->get_root_ptr());
}



void bvh_t::render_frame(unsigned int frame_number)
{
  /* CS775: Implement this method */
    std::list<joint_t *>::const_iterator iterator;
    float data_channels[6] = {};
    int count = 0;
    for(iterator = (*(hierarchy->get_joint_list())).begin(); iterator != (*(hierarchy->get_joint_list())).end(); iterator++) {
        joint_t * j = *iterator;
        int i;
        for(i = 0; i < j->get_channels().num_channels; i++)
            data_channels[i] = motion->get_data()[frame_number][count++];
        j->update_matrix(data_channels);
    }
    render_pose(hierarchy->get_root_ptr());

    /*le Transformer*/    
    body* b;

    /*Variables for Transformer*/
    b->pos_x = data_channels[0];
    b->pos_y = data_channels[1];
    b->pos_z = data_channels[2];

    //b->elbows_y_offset = data_channels[];
    //b->count_elbows_in = data_channels[];
    //b->count_elbows_out = data_channels[];

    //Body Parts
    /*Waist*/
    b->waist_x = data_channels[4];
    b->waist_y = data_channels[5];
    b->waist_z = data_channels[3];

    /*Neck*/
    b->neck_x = data_channels[13];
    b->neck_y = data_channels[14];
    b->neck_z = data_channels[12];

    /*Right Shoulder*/
    b->right_shoulder_x = data_channels[34];
    b->right_shoulder_y = data_channels[35];
    b->right_shoulder_z = data_channels[33];

    /*Left Shoulder*/
    b->left_shoulder_x = data_channels[25];
    b->left_shoulder_y = data_channels[26];
    b->left_shoulder_z = data_channels[24];

    /*Right Wrist*/
    b->right_wrist_x = data_channels[40];
    b->right_wrist_z = data_channels[39];

    /*Left Wrist*/
    b->left_wrist_x = data_channels[31];
    b->left_wrist_z = data_channels[30];

    /*Right Hip*/
    b->right_hip_x = data_channels[55];
    b->right_hip_y = data_channels[56];
    b->right_hip_z = data_channels[54];

    /*Left Hip*/
    b->left_hip_x = data_channels[43];
    b->left_hip_y = data_channels[44];
    b->left_hip_z = data_channels[42];

    /*Left Elbow*/
    b->left_elbow_x = data_channels[28];

    /*Right Elbow*/
    b->right_elbow_x = data_channels[37];

    /*Left Knee*/
    b->left_knee_x = data_channels[46];

    /*Right Knee*/
    b->right_knee_x = data_channels[58];

    /*Left Ankle*/
    b->left_ankle_x = data_channels[49];
    b->left_ankle_y = data_channels[50];

    /*Right Ankle*/
    b->right_ankle_x = data_channels[61];
    b->right_ankle_y = data_channels[62];

    /*Shoulder Joints*/
    b->rs_joint_x = data_channels[31];
    b->ls_joint_x = data_channels[22];
}

double bvh_t::get_max_offset() {
    return hierarchy->get_root_ptr()->get_max_offset();
}

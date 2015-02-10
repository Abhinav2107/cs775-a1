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
    if(!transformer) {
    std::list<joint_t *>::const_iterator iterator;
    float data_channels[6] = {};
    for(iterator = (*(hierarchy->get_joint_list())).begin(); iterator != (*(hierarchy->get_joint_list())).end(); iterator++) {
        joint_t * j = *iterator;
        j->update_matrix(data_channels);
    }
    render_pose(hierarchy->get_root_ptr());
    }
    else {
    b->pos_x = 0;
    b->pos_y = 0;
    b->pos_z = 0;

    //b->elbows_y_offset = 0;
    //b->count_elbows_in = 0;
    //b->count_elbows_out = 0;

    //Body Parts
    //
    b->rotate_x_angle = 0;
    b->rotate_y_angle = 0;
    b->rotate_z_angle = 0;
    /*Waist*/
    b->waist_x = 0;
    b->waist_y = 0;
    b->waist_z = 0;

    /*Neck*/
    b->neck_x = 0;
    b->neck_y = 0;
    b->neck_z = 0;

    /*Right Shoulder*/
    b->right_shoulder_x = 0;
    b->right_shoulder_y = 0;
    b->right_shoulder_z = 0;

    /*Left Shoulder*/
    b->left_shoulder_x = 0;
    b->left_shoulder_y = 0;
    b->left_shoulder_z = 0;

    /*Right Wrist*/
    b->right_wrist_x = 0;
    b->right_wrist_z = 0;

    /*Left Wrist*/
    b->left_wrist_x = 0;
    b->left_wrist_z = 0;

    /*Right Hip*/
    b->right_hip_x = 0;
    b->right_hip_y = 0;
    b->right_hip_z = 0;

    /*Left Hip*/
    b->left_hip_x = 0;
    b->left_hip_y = 0;
    b->left_hip_z = 0;

    /*Left Elbow*/
    b->left_elbow_x = 0;

    /*Right Elbow*/
    b->right_elbow_x = 0;

    /*Left Knee*/
    b->left_knee_x = 0;

    /*Right Knee*/
    b->right_knee_x = 0;

    /*Left Ankle*/
    b->left_ankle_x = 0;
    b->left_ankle_y = 0;

    /*Right Ankle*/
    b->right_ankle_x = 0;
    b->right_ankle_y = 0;

    /*Shoulder Joints*/
    b->rs_joint_x = 0;
    b->ls_joint_x = 0;
    b->render();
    
    }
}



void bvh_t::render_frame(unsigned int frame_number)
{
  /* CS775: Implement this method */
    if(!transformer) {
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
    }
    /*le Transformer*/    
    else {
    /*Variables for Transformer*/
    float * parameters;
    parameters = motion->get_data()[frame_number];
    b->pos_x = (parameters[0] - hierarchy->get_root_ptr()->get_offset()[0])/100;
    b->pos_y = (parameters[1] - hierarchy->get_root_ptr()->get_offset()[1])/100;
    b->pos_z = (parameters[2] - hierarchy->get_root_ptr()->get_offset()[2])/100;

    //b->elbows_y_offset = parameters[];
    //b->count_elbows_in = parameters[];
    //b->count_elbows_out = parameters[];

    //Body Parts
    //

    b->rotate_x_angle = parameters[4] + 0;
    b->rotate_y_angle = parameters[5] + 0;
    b->rotate_z_angle = parameters[3] + 0;
    /*Waist*/
    b->waist_x = parameters[7];
    b->waist_y = parameters[8];
    b->waist_z = parameters[6];
    /*Neck*/
    b->neck_x = parameters[16];
    b->neck_y = parameters[17];
    b->neck_z = parameters[15];

    /*left Shoulder*/
    b->left_shoulder_x = parameters[37];
    b->left_shoulder_y = parameters[38];
    b->left_shoulder_z = parameters[36];

    /*right Shoulder*/
    b->right_shoulder_x = parameters[25];
    b->right_shoulder_y = parameters[26];
    b->right_shoulder_z = parameters[24];

    /*left Wrist*/
    b->left_wrist_x = parameters[43];
    b->left_wrist_z = parameters[42];

    /*right Wrist*/
    b->right_wrist_x = parameters[31];
    b->right_wrist_z = parameters[30];

    /*left Hip*/
    b->left_hip_x = parameters[58];
    b->left_hip_y = parameters[59];
    b->left_hip_z = parameters[57];

    /*right Hip*/
    b->right_hip_x = parameters[46];
    b->right_hip_y = parameters[47];
    b->right_hip_z = parameters[45];

    /*right Elbow*/
    b->right_elbow_x = parameters[28];
    b->right_elbow_y = parameters[29];
    b->right_elbow_z = parameters[27];

    /*left Elbow*/
    b->left_elbow_x = parameters[40];
    b->left_elbow_y = parameters[41];
    b->left_elbow_z = parameters[39];

    /*right Knee*/
    b->right_knee_x = parameters[49];
    b->right_knee_y = parameters[50];
    b->right_knee_z = parameters[48];

    /*left Knee*/
    b->left_knee_x = parameters[61];
    b->left_knee_y = parameters[62];
    b->left_knee_z = parameters[60];

    /*right Ankle*/
    b->right_ankle_x = parameters[52];
    b->right_ankle_y = parameters[53];

    /*left Ankle*/
    b->left_ankle_x = parameters[64];
    b->left_ankle_y = parameters[65];

    /*Shoulder Joints*/
//    b->rs_joint_x = parameters[34];
//    b->ls_joint_x = parameters[22];
    b->render();
    }
}

double bvh_t::get_max_offset() {
    return hierarchy->get_root_ptr()->get_max_offset();
}

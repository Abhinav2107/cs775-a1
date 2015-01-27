#include "main.hpp"
double t = 0;
double p = 0;
int frame = -1;
double r = 50;
double pos_x = 0;
double pos_y = 0;
double pos_z = 0;
int camera = 1;
bool play = false;
void usage(void) 
{
  std::cerr<< "usage: "<<progname<<" [-h] -i bvhfile"<<std::endl<<std::endl;
  std::cerr<< "\t-h\t\t\tprint usage"<<std::endl;  
  std::cerr<< "\t-i bvhfile\t\tThe BVH filename\n";
  exit(0);
}

//-----------------------------------------------------------------------

void renderGL(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-(1.33 * r), 1.33 * r, -(1 * r), 1 * r, r / 2, 10 * r);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float ** data;
    data = bvh_fig->get_motion()->get_data();
    if(frame < 0) {
        pos_x = 0;
        pos_y = 0;
        pos_z = 0;
    }
    else {
        if(camera == 1) {
            pos_x = data[0][0];
            pos_y = data[0][1];
            pos_z = data[0][2];
        }
        else if(camera == 2) {
            pos_x = data[frame][0];
            pos_y = data[frame][1];
            pos_z = data[frame][2];
        }
    }
    gluLookAt(pos_x + 2*r*sin(PI * t / 180)*cos(PI * p / 180), pos_y + 2*r*sin(PI*p/180), pos_z + 2*r*cos(PI*t/180)*cos(PI*p/180), pos_x, pos_y, pos_z, 0, 1, 0);
    if(frame > 0)
        bvh_fig->render_frame(frame);
    else
        bvh_fig->render_canonical_pose();
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glEnd();
}
int main(int argc, char **argv)
{
  progname = argv[0];

  bool bvhf_flag = false;

  char c;
  while ((c = getopt(argc, argv, "h?i:")) != EOF)
    {
      switch (c) 
        {
	case 'h':
        case '?':
          usage();
          break;
	case 'i':
	  bvhfilename =  optarg;   
          bvhf_flag = true;
          break;
	default:
	  usage();
	}
    }

  if (!(bvhf_flag)) usage();

  try 
    { 
      bvh_fig = new bvh::bvh_t(bvhfilename, true); 
      try 
	{ 
	  bvh_fig->print_hierarchy(std::cout); 
      r = bvh_fig->get_max_offset();
	}
      catch (util::common::error *e)
	{ util::common::error::halt_on_error(e); }

      /*!
       * CS 775:
       * CALL THE FUNCTION THAT STARTS YOUR PROGRAM HERE
       */

    GLFWwindow * window;
    int height = 750;
    int width = 1000;
    glfwSetErrorCallback(error_callback);
    if(!glfwInit())
        return -1;
    window = glfwCreateWindow(width, height, "BVH Player", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        std::cerr << "GLEW Init Failed" << std::endl;
    }
    glViewport(0, 0, width, height);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    initGL();
    double time;
    double step = (bvh_fig->get_motion()->get_frame_rate());
    int frames = bvh_fig->get_motion()->get_frames();
    while(glfwWindowShouldClose(window) == 0) {
        if(play && frame < frames-1)
            frame++;
        time = glfwGetTime();
        renderGL();
        glfwSwapBuffers(window);
        glfwPollEvents();
                while(glfwGetTime() < time + step);
    }
    }
  catch (util::common::error *e)
    { util::common::error::halt_on_error(e); }

  return 0;
}

//-----------------------------------------------------------------------

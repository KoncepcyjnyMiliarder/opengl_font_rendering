#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <generic_shader.hpp>
#include <font.hpp>
#include <font_renderer.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main()
{
  if (glfwInit() == GLFW_FALSE)
  {
    std::cerr << "glfwInit fail\n";
    return 1;
  }
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* mywnd = glfwCreateWindow(800, 600, "font playground", nullptr, nullptr);
  glfwMakeContextCurrent(mywnd);
  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
    std::cerr << "glewInit fail\n";
    return 1;
  }
  //init buffers

  generic_shader color("rsrc/color.vs", "rsrc/color.fs");
  font myfont(font_loader("rsrc/arial_15.fnt"));
  font boldfont(font_loader("rsrc/ebrima_24_bold.fnt"));

  font_renderer fr;
  std::wstring mystr(L"english ");
  for (wchar_t i = 260; i < 290; ++i)
    mystr.push_back(i);
  for (wchar_t i = 360; i < 382; ++i)
    mystr.push_back(i);
  cached_text longtext(
    L"Lorem Ipsum is simply dummy text of the printing and typesetting industry. "
    "Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, "
    "when an unknown printer took a galley of type and scrambled it to make a type specimen book. "
    "It has survived not only five centuries, but also the leap into electronic typesetting, "
    "remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset "
    "sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like "
    "Aldus PageMaker including versions of Lorem Ipsum."
    "Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of "
    "classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin "
    "professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, "
    "consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical "
    "literature, discovered the undoubtable source. Lorem Ipsum comes from sections 1.10.32 and 1.10.33 "
    "of \"de Finibus Bonorum et Malorum\" (The Extremes of Good and Evil) by Cicero, written in 45 BC. "
    "This book is a treatise on the theory of ethics, very popular during the Renaissance. The first "
    "line of Lorem Ipsum, \"Lorem ipsum dolor sit amet..\", comes from a line in section 1.10.32.", myfont, 400, 600);
  cached_text various_glyphs(mystr, myfont);
  cached_text number_string(L"123.45453956356358736589735698357643895735689375683975683945746359", boldfont, 200, 200);

  float quadVertices[] =
  {
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
  };
  unsigned int quadVAO, quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  generic_shader blur_shader("rsrc/blur.vs", "rsrc/blur.fs");
  generic_shader screen_shader("rsrc/screen.vs", "rsrc/screen.fs");
  // framebuffer configuration

  unsigned int framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  unsigned int textureColorbuffer;
  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  float time = glfwGetTime();
  int counter = 0;
  bool use_blur = false;

  while (!glfwWindowShouldClose(mywnd))
  {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    ++counter;
    if (time + 1.0 < glfwGetTime())
    {
      printf("%d\n", counter);
      counter = 0;
      use_blur = !use_blur;
      time = glfwGetTime();
    }
    glfwPollEvents();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    //text to framebfufer
    fr.use(glm::ortho<float>(0, 800, 600, 0));
    fr.draw_text(various_glyphs, 0, 0, glm::vec3(0.5f, 0.5f, 0.5f));
    fr.draw_text(number_string, 0, 10, glm::vec3(1.0f, fmod(glfwGetTime(), 1.0f), 1.0f));
    fr.draw_text(longtext, 200, 100, glm::vec3(0.2f, 0.9f, 0.5f));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //postprocessing
    if (use_blur)
    {
      blur_shader.use();
      glBindVertexArray(quadVAO);
      glActiveTexture(GL_TEXTURE0);
      blur_shader.set_uint_uniform("screenTexture", 0);
    }
    else
    {
      screen_shader.use();
      glBindVertexArray(quadVAO);
      glActiveTexture(GL_TEXTURE0);
      screen_shader.set_uint_uniform("screenTexture", 0);
    }
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(mywnd);
  }
  glfwDestroyWindow(mywnd);
  glfwTerminate();
  return 0;
}
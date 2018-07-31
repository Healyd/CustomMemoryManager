//#include "ParticleSystemPCH.h"
//#include "ElapsedTime.h"
//#include "PivotCamera.h"
//#include "ParticleEffect.h"
//#include "SphereEmitter.h"
//#include "CubeEmitter.h"
//#include "Util.h"
//
//#include "imgui-1.61/imgui.h"
//#include "imgui_impl_glfw_gl3.h"
//#include <stdio.h>
//#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
//
//
//static void glfw_error_callback(int error, const char* description)
//{
//	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
//}
//
////static void glfw_error_callback(int error, const char* description)
////{
////	fprintf(stderr, "Error %d: %s\n", error, description);
////}
//
//PivotCamera g_Camera;
//SphereEmitter g_ParticleEmitter;
//CubeEmitter g_CubeEmitter;
//
////#if _DEBUG
////ParticleEffect g_ParticleEffect(1000);
////#else
////ParticleEffect g_ParticleEffect(100000);
////#endif 
//ParticleEffect g_ParticleEffect;
//
//int g_iWindowWidth = 1280;
//int g_iWindowHeight = 720;
//int g_iGLUTWindowHandle = 0;
//int g_iErrorCode = 0;
//
//bool g_bLeftMouseDown = false;
//bool g_bRightMouseDown = false;
//
//bool g_bUpdate = true;
//
//glm::vec2 g_MouseCurrent = glm::vec2(0);
//glm::vec2 g_MousePrevious = glm::vec2(0);
//glm::vec2 g_MouseDelta = glm::vec2(0);
//
//glm::vec3 g_DefaultCameraTranslate(0, 0, 100);
//glm::vec3 g_DefaultCameraRotate(40, 0, 0);
//glm::vec3 g_DefaultCameraPivot(0, 0, 0);
//
//void InitGL(int argc, char* argv[]);
//void DisplayGL();
//void IdleGL();
//void SpecialKeysGL(int c, int x, int y);
//void KeyboardGL(unsigned char c, int x, int y);
//void MouseGL(int button, int state, int x, int y);
//void MotionGL(int x, int y);
//void ReshapeGL(int w, int h);
//
//// We're exiting, cleanup the allocated resources.
//void Cleanup();
//
//bool doOnce = true;
//
//int main(int argc, char* argv[])
//{
//	//	glfwInit();
//	//	GLFWwindow* window = glfwCreateWindow(1280, 720, "Particle Systems", nullptr, nullptr);
//	//	if (window == nullptr)
//	//	{
//	//		std::cout << "Failed to create GLFW window" << std::endl;
//	//		glfwTerminate();
//	//		return -1;
//	//	}
//	//
//	//	const char* glsl_version = "#version 130";
//	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//	//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	//
//	//	glfwMakeContextCurrent(window);
//	//	glfwSwapInterval(1);
//	//
//	//
//	//	// Setup Dear ImGui binding
//	//	//IMGUI_CHECKVERSION();
//	//	ImGui::CreateContext();
//	//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	//
//	//	ImGui_ImplGlfw_InitForOpenGL(window, true);
//	//	ImGui_ImplOpenGL3_Init(glsl_version);
//	//	ImGui::StyleColorsDark();
//	//
//	//	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//	//	//glfwSetCursorPosCallback(window, mouse_callback);
//	//	//glfwSetScrollCallback(window, scroll_callback);
//	//
//	//	// tell GLFW to capture our mouse
//	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	//
//	//	if (doOnce)
//	//	{
//	//		GLOBAL_MEMORY_MANAGER.CreateAllocator("test", 10000000 * sizeof(Particle), CustomMemoryManager::AllocType::HEAP);
//	//		//GLOBAL_MEMORY_MANAGER.CreateAllocator<Particle>("test", 100000 * sizeof(Particle), CustomMemoryManager::AllocType::POOL);
//	////#if _DEBUG
//	//		//g_ParticleEffect.Resize(1000);
//	////#else
//	////#endif 
//	//		g_ParticleEffect.Resize(100000);
//	//		doOnce = false;
//	//	}
//	//
//	//	/*bool createParticles = false;
//	//	bool deleteParticles = false;
//	//
//	//	if (g_ParticleEffect.GetNumParticles() <= 0)
//	//	{
//	//		createParticles = true;
//	//	}
//	//	else if (g_ParticleEffect.GetNumParticles() >= 100000)
//	//	{
//	//		createParticles = false;
//	//		deleteParticles = true;
//	//	}
//	//	else
//	//	{
//	//		if (getRangedRandom(0, 100) < g_ParticleEffect.getCreateProbability())
//	//		{
//	//			createParticles = true;
//	//			createParticles = false;
//	//		}
//	//	}
//	//	if (createParticles)
//	//	{
//	//		g_ParticleEffect.CreateParticle();
//	//	}
//	//	if (deleteParticles)
//	//	{
//	//		g_ParticleEffect.DestroyParticle();
//	//	}*/
//	//
//	//	//InitGL(argc, argv);
//	//
//	//	g_Camera.SetTranslate(g_DefaultCameraTranslate);
//	//	g_Camera.SetRotate(g_DefaultCameraRotate);
//	//	g_Camera.SetPivot(g_DefaultCameraPivot);
//	//
//	//	if (g_ParticleEffect.LoadTexture("Data/Textures/Particle-Texture.png"))
//	//	{
//	//		std::cout << "Successfully loaded particle texture." << std::endl;
//	//	}
//	//	else
//	//	{
//	//		std::cerr << "Failed to load particle texture!" << std::endl;
//	//	}
//	//	ParticleEffect::ColorInterpolator colors;
//	//
//	//	//colors.AddValue(0.0f, glm::vec4(1, 0, 0, 1));     // red
//	//	//colors.AddValue(0.15f, glm::vec4(1, 0, 1, 1));     // magenta
//	//	//colors.AddValue(0.33f, glm::vec4(0, 0, 1, 1));     // blue
//	//	//colors.AddValue(0.5f, glm::vec4(0, 1, 1, 1));     // cyan
//	//	//colors.AddValue(0.67f, glm::vec4(0, 1, 0, 0.75));  // green
//	//	//colors.AddValue(0.84f, glm::vec4(1, 1, 0, 0.5));   // yellow
//	//	//colors.AddValue(1.0f, glm::vec4(1, 0, 0, 0));     // red
//	//
//	//	colors.AddValue(0.0f, glm::vec4(1, 0, 0, 1));     // red
//	//	colors.AddValue(2.0f, glm::vec4(1, 0, 1, 1));     // magenta
//	//	colors.AddValue(3.0f, glm::vec4(0, 0, 1, 1));     // blue
//	//	colors.AddValue(4.0f, glm::vec4(0, 1, 1, 1));     // cyan
//	//	colors.AddValue(5.0f, glm::vec4(0, 1, 0, 0.75));  // green
//	//	colors.AddValue(6.0f, glm::vec4(1, 1, 0, 0.5));   // yellow
//	//
//	//	g_ParticleEffect.SetColorInterplator(colors);
//	//
//	//	g_ParticleEffect.SetParticleEmitter(&g_ParticleEmitter);
//	//	g_ParticleEffect.EmitParticles();
//	//	g_ParticleEffect.SetCamera(&g_Camera);
//	//
//	//	//while (1)
//	//	//{
//	//	//	static ElapsedTime elapsedTime;
//	//	//	float fDeltaTime = elapsedTime.GetElapsedTime();
//	//	//	glfwPollEvents();
//	//
//	//	//	// Start the Dear ImGui frame
//	//	//	ImGui_ImplOpenGL3_NewFrame();
//	//	//	ImGui_ImplGlfw_NewFrame();
//	//	//	ImGui::NewFrame();
//	//	//	if (g_bUpdate)
//	//	//	{
//	//	//		g_ParticleEffect.Update(fDeltaTime);
//	//	//		std::cout << g_ParticleEffect.GetNumParticles() << std::endl;
//	//	//	}
//	//	//	else
//	//	//	{
//	//	//		g_ParticleEffect.BuildVertexBuffer();
//	//	//	}
//	//	//}
//	//
//	//	while (!glfwWindowShouldClose(window))
//	//	{
//	//		// per-frame time logic
//	//		// --------------------
//	//		float currentFrame = glfwGetTime();
//	//		//deltaTime = currentFrame - lastFrame;
//	//		//lastFrame = currentFrame;
//	//
//	//		//float fps = 1.0f / deltaTime;
//	//		glfwPollEvents();
//	//
//	//		// Start the Dear ImGui frame
//	//		ImGui_ImplOpenGL3_NewFrame();
//	//		ImGui_ImplGlfw_NewFrame();
//	//		ImGui::NewFrame();
//	//
//	//		//if (show_demo_window)
//	//		//	ImGui::ShowDemoWindow(&show_demo_window);
//	//
//	//		{
//	//			static float f = 0.0f;
//	//			static int counter = 0;
//	//
//	//			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//	//
//	//			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//	//			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//	//																	//ImGui::Checkbox("Another Window", &show_another_window);
//	//
//	//			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
//	//																	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//	//
//	//			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//	//				counter++;
//	//			ImGui::SameLine();
//	//			ImGui::Text("counter = %d", counter);
//	//
//	//			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//	//			ImGui::End();
//	//		}
//	//
//	//		//std::cout << fps << std::endl;
//	//
//	//		// input
//	//		// -----
//	//		//processInput(window);
//	//
//	//		// render
//	//		// ------
//	//		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//	//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	//
//	//		// bind textures on corresponding texture units
//	//		//glActiveTexture(GL_TEXTURE0);
//	//		//glBindTexture(GL_TEXTURE_2D, texture1);
//	//
//	//		//shader.use();
//	//
//	//		// configure transformation matrices
//	//		//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000000.0f);
//	//		// camera/view transformation
//	//		//glm::mat4 view = camera.GetViewMatrix();
//	//		glm::mat4 model;
//	//
//	//		//glm::mat4 viewProjection = projection * view * model;
//	//		//shader.setMat4("viewProjection", viewProjection);
//	//
//	//		//for (const auto& effect : effects)
//	//		//{
//	//			//effect->Update(deltaTime);
//	//		//}
//	//
//	//		//ParticleEffect::Render(effects);
//	//		ImGui::Render();
//	//		int display_w, display_h;
//	//		glfwMakeContextCurrent(window);
//	//		glfwGetFramebufferSize(window, &display_w, &display_h);
//	//		glViewport(0, 0, display_w, display_h);
//	//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//	//
//	//		glfwMakeContextCurrent(window);
//	//		glfwSwapBuffers(window);
//	//
//	//		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//	//		// -------------------------------------------------------------------------------
//	//		//glfwSwapBuffers(window);
//	//	}
//	//
//	//	glfwTerminate();
//	//	return 0;
//
//	// Setup window
//	glfwSetErrorCallback(glfw_error_callback);
//	if (!glfwInit())
//		return 1;
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#if __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//	GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui GLFW+OpenGL3 example", NULL, NULL);
//	glfwMakeContextCurrent(window);
//	glfwSwapInterval(1); // Enable vsync
//	//gl3wInit();
//	glewInit();
//
//	// Setup Dear ImGui binding
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
//	ImGui_ImplGlfwGL3_Init(window, true);
//
//	// Setup style
//	ImGui::StyleColorsDark();
//	//ImGui::StyleColorsClassic();
//
//	// Load Fonts
//	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
//	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
//	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//	// - Read 'misc/fonts/README.txt' for more instructions and details.
//	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//	//io.Fonts->AddFontDefault();
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
//	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
//	//IM_ASSERT(font != NULL);
//
//	bool show_demo_window = true;
//	bool show_another_window = false;
//	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//		if (doOnce)
//		{
//			GLOBAL_MEMORY_MANAGER.CreateAllocator("test", 10000000 * sizeof(Particle), CustomMemoryManager::AllocType::HEAP);
//			//GLOBAL_MEMORY_MANAGER.CreateAllocator<Particle>("test", 100000 * sizeof(Particle), CustomMemoryManager::AllocType::POOL);
//	//#if _DEBUG
//			//g_ParticleEffect.Resize(1000);
//	//#else
//	//#endif 
//			g_ParticleEffect.Resize(100000);
//			doOnce = false;
//		}
//	
//		g_Camera.SetTranslate(g_DefaultCameraTranslate);
//		g_Camera.SetRotate(g_DefaultCameraRotate);
//		g_Camera.SetPivot(g_DefaultCameraPivot);
//	
//		if (g_ParticleEffect.LoadTexture("Data/Textures/Particle-Texture.png"))
//		{
//			std::cout << "Successfully loaded particle texture." << std::endl;
//		}
//		else
//		{
//			std::cerr << "Failed to load particle texture!" << std::endl;
//		}
//		ParticleEffect::ColorInterpolator colors;
//	
//		colors.AddValue(0.0f, glm::vec4(1, 0, 0, 1));     // red
//		colors.AddValue(2.0f, glm::vec4(1, 0, 1, 1));     // magenta
//		colors.AddValue(3.0f, glm::vec4(0, 0, 1, 1));     // blue
//		colors.AddValue(4.0f, glm::vec4(0, 1, 1, 1));     // cyan
//		colors.AddValue(5.0f, glm::vec4(0, 1, 0, 0.75));  // green
//		colors.AddValue(6.0f, glm::vec4(1, 1, 0, 0.5));   // yellow
//	
//		g_ParticleEffect.SetColorInterplator(colors);
//	
//		g_ParticleEffect.SetParticleEmitter(&g_ParticleEmitter);
//		g_ParticleEffect.EmitParticles();
//		g_ParticleEffect.SetCamera(&g_Camera);
//
//	// Main loop
//	while (!glfwWindowShouldClose(window))
//	{
//		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
//		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
//		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
//		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
//		glfwPollEvents();
//		ImGui_ImplGlfwGL3_NewFrame();
//
//		// 1. Show a simple window.
//		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
//		{
//			static float f = 0.0f;
//			static int counter = 0;
//			ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
//			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
//			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
//			ImGui::Checkbox("Another Window", &show_another_window);
//
//			if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
//				counter++;
//			ImGui::SameLine();
//			ImGui::Text("counter = %d", counter);
//
//			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//		}
//
//		// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
//		if (show_another_window)
//		{
//			ImGui::Begin("Another Window", &show_another_window);
//			ImGui::Text("Hello from another window!");
//			if (ImGui::Button("Close Me"))
//				show_another_window = false;
//			ImGui::End();
//		}
//
//		// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
//		if (show_demo_window)
//		{
//			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
//			ImGui::ShowDemoWindow(&show_demo_window);
//		}
//
//		// Rendering
//		int display_w, display_h;
//		glfwGetFramebufferSize(window, &display_w, &display_h);
//		glViewport(0, 0, display_w, display_h);
//		//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//		//glClear(GL_COLOR_BUFFER_BIT);
//		ImGui::Render();
//		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
//		glfwSwapBuffers(window);
//
//		static ElapsedTime elapsedTime;
//		float fDeltaTime = elapsedTime.GetElapsedTime();
//
//		// Start the Dear ImGui frame
//		if (g_bUpdate)
//		{
//			g_ParticleEffect.Update(fDeltaTime);
//			std::cout << g_ParticleEffect.GetNumParticles() << std::endl;
//		}
//		else
//		{
//			g_ParticleEffect.BuildVertexBuffer();
//		}
//	}
//
//	// Cleanup
//	ImGui_ImplGlfwGL3_Shutdown();
//	ImGui::DestroyContext();
//
//	glfwDestroyWindow(window);
//	glfwTerminate();
//
//	return 0;
//
//	//glutMainLoop();
//}
//
//void Cleanup()
//{
//	if (g_iGLUTWindowHandle != 0)
//	{
//		glutDestroyWindow(g_iGLUTWindowHandle);
//		g_iGLUTWindowHandle = 0;
//	}
//}
//
//
//void InitGL(int argc, char* argv[])
//{
//	std::cout << "Initialise OpenGL..." << std::endl;
//
//	glutInit(&argc, argv);
//	int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
//	int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);
//
//	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
//
//	glutInitWindowPosition((iScreenWidth - g_iWindowWidth) / 2,
//		(iScreenHeight - g_iWindowHeight) / 2);
//	glutInitWindowSize(g_iWindowWidth, g_iWindowHeight);
//
//	g_iGLUTWindowHandle = glutCreateWindow("OpenGL");
//
//	// Register GLUT callbacks
//	glutDisplayFunc(DisplayGL);
//	glutIdleFunc(IdleGL);
//	glutMouseFunc(MouseGL);
//	glutMotionFunc(MotionGL);
//	glutSpecialFunc(SpecialKeysGL);
//	glutKeyboardFunc(KeyboardGL);
//	glutReshapeFunc(ReshapeGL);
//
//	// Setup initial GL State
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClearDepth(1.0f);
//
//	glShadeModel(GL_SMOOTH);
//
//	std::cout << "Initialise OpenGL: Success!" << std::endl;
//}
//
//void DrawAxis(float fScale, glm::vec3 translate = glm::vec3(0))
//{
//	glPushAttrib(GL_ENABLE_BIT);
//
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_LIGHTING);
//
//	glPushMatrix();
//	glTranslatef(translate.x, translate.y, translate.z);
//	glScalef(fScale, fScale, fScale);
//	glColor3f(0.0f, 0.0f, 1.0f);
//
//	glBegin(GL_LINES);
//	{
//		glColor3f(1.0f, 0.0f, 0.0f);
//		glVertex3f(0.0f, 0.0f, 0.0);
//		glVertex3f(1.0f, 0.0f, 0.0f);
//
//		glColor3f(0.0f, 1.0f, 0.0f);
//		glVertex3f(0.0f, 0.0f, 0.0f);
//		glVertex3f(0.0f, 1.0f, 0.0f);
//
//		glColor3f(0.0f, 0.0f, 1.0f);
//		glVertex3f(0.0f, 0.0f, 0.0f);
//		glVertex3f(0.0f, 0.0f, 1.0f);
//	}
//	glEnd();
//
//	glPopMatrix();
//	glPopAttrib();
//}
//
//void DisplayGL()
//{
//	glfwPollEvents();
//
//	// Start the Dear ImGui frame
//	//ImGui_ImplOpenGL3_NewFrame();
//	//ImGui_ImplGlfw_NewFrame();
//	//ImGui::NewFrame();
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                   // Clear the color buffer, and the depth buffer.
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	g_Camera.ApplyViewTransform();
//
//	DrawAxis(20.0f, g_Camera.GetPivot());
//
//	g_ParticleEffect.Render();
//
//
//	glutSwapBuffers();
//	glutPostRedisplay();
//}
//
//void IdleGL()
//{
//	static ElapsedTime elapsedTime;
//	float fDeltaTime = elapsedTime.GetElapsedTime();
//
//	if (g_bUpdate)
//	{
//		g_ParticleEffect.Update(fDeltaTime);
//		std::cout << g_ParticleEffect.GetNumParticles() << std::endl;
//	}
//	else
//	{
//		g_ParticleEffect.BuildVertexBuffer();
//	}
//
//	glutPostRedisplay();
//}
//
//template< typename T >
//void Increment(T& value, const T& step, const T& max)
//{
//	value = std::min(max, value + step);
//}
//
//template< typename T >
//void Decrement(T& value, const T& step, const T& min)
//{
//	value = std::max(min, value - step);
//}
//
//void SpecialKeysGL(int c, int x, int y)
//{
//	const float fStep = 1.0f;
//	switch (c)
//	{
//	case GLUT_KEY_UP:
//	{
//		Increment(g_CubeEmitter.MaxWidth, fStep, 100.0f);
//		Increment(g_CubeEmitter.MaxHeight, fStep, 100.0f);
//		Increment(g_CubeEmitter.MaxDepth, fStep, 100.0f);
//		g_CubeEmitter.MinWidth = -g_CubeEmitter.MaxWidth;
//		g_CubeEmitter.MinHeight = -g_CubeEmitter.MaxHeight;
//		g_CubeEmitter.MinDepth = -g_CubeEmitter.MaxDepth;
//
//		Increment(g_ParticleEmitter.MinInclination, fStep, 180.0f);
//		Decrement(g_ParticleEmitter.MaxInclination, fStep, g_ParticleEmitter.MinInclination);
//	}
//	break;
//	case GLUT_KEY_DOWN:
//	{
//		Decrement(g_CubeEmitter.MaxWidth, fStep, 0.0f);
//		Decrement(g_CubeEmitter.MaxHeight, fStep, 0.0f);
//		Decrement(g_CubeEmitter.MaxDepth, fStep, 0.0f);
//
//		g_CubeEmitter.MinWidth = -g_CubeEmitter.MaxWidth;
//		g_CubeEmitter.MinHeight = -g_CubeEmitter.MaxHeight;
//		g_CubeEmitter.MinDepth = -g_CubeEmitter.MaxDepth;
//
//		Decrement(g_ParticleEmitter.MinInclination, fStep, 0.0f);
//		Increment(g_ParticleEmitter.MaxInclination, fStep, 180.0f);
//	}
//	break;
//	case GLUT_KEY_LEFT:
//	{
//		Increment(g_ParticleEmitter.MinAzimuth, fStep, 360.0f);
//		Decrement(g_ParticleEmitter.MaxAzimuth, fStep, 0.0f);
//	}
//	break;
//	case GLUT_KEY_RIGHT:
//	{
//		Decrement(g_ParticleEmitter.MinAzimuth, fStep, 0.0f);
//		Increment(g_ParticleEmitter.MaxAzimuth, fStep, 360.0f);
//	}
//	break;
//	};
//
//	glutPostRedisplay();
//}
//
//void KeyboardGL(unsigned char c, int x, int y)
//{
//	float fRadiusStep = 1.0f;
//
//	switch (c)
//	{
//	case ' ': // Space bar
//	{
//		// Toggle updating of the simulation
//		g_bUpdate = !g_bUpdate;
//	}
//	break;
//	case '-':
//	{
//		g_ParticleEmitter.MaximumRadius -= fRadiusStep;
//		g_ParticleEmitter.MinimumRadius -= fRadiusStep;
//
//		g_ParticleEmitter.MaximumRadius = std::max(0.0f, g_ParticleEmitter.MaximumRadius);
//		g_ParticleEmitter.MaximumRadius = std::max(0.0f, g_ParticleEmitter.MaximumRadius);
//	}
//	break;
//	case '+':
//	{
//		g_ParticleEmitter.MaximumRadius += fRadiusStep;
//		g_ParticleEmitter.MinimumRadius += fRadiusStep;
//
//		g_ParticleEmitter.MaximumRadius = std::min(200.0f, g_ParticleEmitter.MaximumRadius);
//		g_ParticleEmitter.MaximumRadius = std::min(200.0f, g_ParticleEmitter.MaximumRadius);
//	}
//	break;
//	case 's':
//	case 'S':
//	{
//		std::cout << "Shade Model: GL_SMOOTH" << std::endl;
//		// Switch to smooth shading model
//		glShadeModel(GL_SMOOTH);
//	}
//	break;
//	case 'f':
//	case 'F':
//	{
//		std::cout << "Shade Model: GL_FLAT" << std::endl;
//		// Switch to flat shading model
//		glShadeModel(GL_FLAT);
//	}
//	break;
//	case 'r':
//	case 'R':
//	{
//		std::cout << "Reset Parameters..." << std::endl;
//		g_Camera.SetTranslate(g_DefaultCameraTranslate);
//		g_Camera.SetRotate(g_DefaultCameraRotate);
//		g_Camera.SetPivot(g_DefaultCameraPivot);
//	}
//	break;
//	case '\033': // escape quits
//	case '\015': // Enter quits    
//	case 'Q':    // Q quits
//	case 'q':    // q (or escape) quits
//	{
//		// Cleanup up and quit
//		exit(0);
//	}
//	break;
//	}
//
//	glutPostRedisplay();
//}
//
//void MouseGL(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON)
//	{
//		g_bLeftMouseDown = (state == GLUT_DOWN);
//	}
//	else if (button == GLUT_RIGHT_BUTTON)
//	{
//		g_bRightMouseDown = (state == GLUT_DOWN);
//	}
//	g_MousePrevious = glm::vec2(x, y);
//
//}
//
//void MotionGL(int x, int y)
//{
//	g_MouseCurrent = glm::vec2(x, y);
//	g_MouseDelta = (g_MousePrevious - g_MouseCurrent);
//
//	// Update the camera
//	if (g_bLeftMouseDown && g_bRightMouseDown)
//	{
//		g_Camera.TranslateZ(g_MouseDelta.y);
//	}
//	else if (g_bLeftMouseDown)
//	{
//		g_Camera.AddPitch(-g_MouseDelta.y);
//		g_Camera.AddYaw(-g_MouseDelta.x);
//	}
//	else if (g_bRightMouseDown)
//	{
//		g_Camera.TranslatePivotX(g_MouseDelta.x);
//		g_Camera.TranslatePivotY(-g_MouseDelta.y);
//	}
//
//	g_MousePrevious = g_MouseCurrent;
//}
//
//void ReshapeGL(int w, int h)
//{
//	std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;
//
//	h = std::max(h, 1);
//
//	g_iWindowWidth = w;
//	g_iWindowHeight = h;
//
//	g_Camera.SetViewport(0, 0, w, h);
//	g_Camera.ApplyViewport();
//
//	g_Camera.SetProjection(60.0f, w / (float)h, 0.1f, 1000.0f);
//	g_Camera.ApplyProjectionTransform();
//
//	glutPostRedisplay();
//}



/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ParticleSystemPCH.h"
#include "ElapsedTime.h"
#include "PivotCamera.h"
#include "ParticleEffect.h"
#include "SphereEmitter.h"
#include "CubeEmitter.h"
#include "MemoryGui.h"

//new
#include "imgui-1.61/imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

PivotCamera g_Camera;
SphereEmitter g_ParticleEmitter;
CubeEmitter g_CubeEmitter;

//#if _DEBUG
//ParticleEffect g_ParticleEffect(1000);
//#else
//ParticleEffect g_ParticleEffect(100000);
//#endif 

ParticleEffect g_ParticleEffect;
#ifdef _MEMDEBUG
CustomMemoryManager::MemoryGui g_MemoryGui(GLOBAL_MEMORY_MANAGER);
#endif // _MEMDEBUG


int g_iWindowWidth = 1280;
int g_iWindowHeight = 720;
int g_iGLUTWindowHandle = 0;
int g_iErrorCode = 0;

bool g_bLeftMouseDown = false;
bool g_bRightMouseDown = false;

bool g_bUpdate = true;

glm::vec2 g_MouseCurrent = glm::vec2(0);
glm::vec2 g_MousePrevious = glm::vec2(0);
glm::vec2 g_MouseDelta = glm::vec2(0);

glm::vec3 g_DefaultCameraTranslate(0, 0, 100);
glm::vec3 g_DefaultCameraRotate(40, 0, 0);
glm::vec3 g_DefaultCameraPivot(0, 0, 0);

void InitGL(int argc, char* argv[]);
void DisplayGL();
void IdleGL();
void SpecialKeysGL(int c, int x, int y);
void KeyboardGL(unsigned char c, int x, int y);
void MouseGL(int button, int state, int x, int y);
void MotionGL(int x, int y);
void ReshapeGL(int w, int h);

// We're exiting, cleanup the allocated resources.
void Cleanup();

GLFWwindow* window;// = glfwCreateWindow(1280, 720, "ImGui GLFW+OpenGL3 example", NULL, NULL);
#include "StopWatch.h"
int main(int argc, char* argv[])
{
	InitGL(argc, argv);




#if USE_HEAP_FIRST == 1 || USE_HEAP_BEST == 1 || USE_HEAP_LAST == 1
	GLOBAL_MEMORY_MANAGER.CreateAllocator("test", 200000 * sizeof(Particle), CustomMemoryManager::AllocType::HEAP);
#elif USE_POOL == 1
	GLOBAL_MEMORY_MANAGER.CreateAllocator<Particle>("test", 200000 * sizeof(Particle), CustomMemoryManager::AllocType::POOL);
#endif
	//g_ParticleEffect.Resize(100000);
#ifdef _MEMDEBUG
	g_MemoryGui.Init();
#endif // _MEMDEBUG
	/*g_ParticleEffect.LooseRefToAllParticles();
	CustomMemoryManager::Allocators::IAllocator* allocator = GLOBAL_MEMORY_MANAGER.Get("test", CustomMemoryManager::AllocType::HEAP);
	if (allocator != nullptr)
	{
		CustomMemoryManager::Allocators::HeapAllocator* heap = static_cast<CustomMemoryManager::Allocators::HeapAllocator*>(allocator);
		if (heap != nullptr)
		{
			Library::StopWatch sw;
			sw.Start();
			heap->ReferenceCounting_GC();
			sw.Start();
			float x = sw.Elapsed().count();
			std::cout << "here: " << x << std::endl;
		}
	}*/


	g_Camera.SetTranslate(g_DefaultCameraTranslate);
	g_Camera.SetRotate(g_DefaultCameraRotate);
	g_Camera.SetPivot(g_DefaultCameraPivot);

	if (g_ParticleEffect.LoadTexture("Data/Textures/Particle-Texture.png"))
	{
		std::cout << "Successfully loaded particle texture." << std::endl;
	}
	else
	{
		std::cerr << "Failed to load particle texture!" << std::endl;
	}
	//new
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(1280, 720, "ImGui GLFW+OpenGL3 example", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
	glewInit();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
	ParticleEffect::ColorInterpolator colors;

	//colors.AddValue(0.0f, glm::vec4(1, 0, 0, 1));     // red
	//colors.AddValue(0.15f, glm::vec4(1, 0, 1, 1));     // magenta
	//colors.AddValue(0.33f, glm::vec4(0, 0, 1, 1));     // blue
	//colors.AddValue(0.5f, glm::vec4(0, 1, 1, 1));     // cyan
	//colors.AddValue(0.67f, glm::vec4(0, 1, 0, 0.75));  // green
	//colors.AddValue(0.84f, glm::vec4(1, 1, 0, 0.5));   // yellow
	//colors.AddValue(1.0f, glm::vec4(1, 0, 0, 0));     // red

	colors.AddValue(0.0f, glm::vec4(1, 0, 0, 1));     // red
	colors.AddValue(2.0f, glm::vec4(1, 0, 1, 1));     // magenta
	colors.AddValue(3.0f, glm::vec4(0, 0, 1, 1));     // blue
	colors.AddValue(4.0f, glm::vec4(0, 1, 1, 1));     // cyan
	colors.AddValue(5.0f, glm::vec4(0, 1, 0, 0.75));  // green
	colors.AddValue(6.0f, glm::vec4(1, 1, 0, 0.5));   // yellow

	g_ParticleEffect.SetColorInterplator(colors);

	g_ParticleEffect.SetParticleEmitter(&g_ParticleEmitter);
	g_ParticleEffect.EmitParticles();
	g_ParticleEffect.SetCamera(&g_Camera);

	glutMainLoop();
}

void Cleanup()
{
	if (g_iGLUTWindowHandle != 0)
	{
		glutDestroyWindow(g_iGLUTWindowHandle);
		g_iGLUTWindowHandle = 0;
	}
}


void InitGL(int argc, char* argv[])
{
	std::cout << "Initialise OpenGL..." << std::endl;

	glutInit(&argc, argv);
	int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition((iScreenWidth - g_iWindowWidth) / 2,
		(iScreenHeight - g_iWindowHeight) / 2);
	glutInitWindowSize(g_iWindowWidth, g_iWindowHeight);

	g_iGLUTWindowHandle = glutCreateWindow("OpenGL");
	//window = glfwCreateWindow(1280, 720, "ImGui GLFW+OpenGL3 example", NULL, NULL);

	// Register GLUT callbacks
	glutDisplayFunc(DisplayGL);
	glutIdleFunc(IdleGL);
	glutMouseFunc(MouseGL);
	glutMotionFunc(MotionGL);
	glutSpecialFunc(SpecialKeysGL);
	glutKeyboardFunc(KeyboardGL);
	glutReshapeFunc(ReshapeGL);

	// Setup initial GL State
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	glShadeModel(GL_SMOOTH);

	std::cout << "Initialise OpenGL: Success!" << std::endl;
}

void DrawAxis(float fScale, glm::vec3 translate = glm::vec3(0))
{
	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(translate.x, translate.y, translate.z);
	glScalef(fScale, fScale, fScale);
	glColor3f(0.0f, 0.0f, 1.0f);

	glBegin(GL_LINES);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0);
		glVertex3f(1.0f, 0.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
	}
	glEnd();

	glPopMatrix();
	glPopAttrib();
}

void DisplayGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                   // Clear the color buffer, and the depth buffer.

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	g_Camera.ApplyViewTransform();

	DrawAxis(20.0f, g_Camera.GetPivot());

	g_ParticleEffect.Render();

	glutSwapBuffers();
	glutPostRedisplay();
}

bool createParticles = true;
bool deleteParticles = true;
int amount = 10;

void IdleGL()
{
	static ElapsedTime elapsedTime;
	float fDeltaTime = elapsedTime.GetElapsedTime();

	//new
	glfwPollEvents();
	ImGui_ImplGlfwGL3_NewFrame();
#ifdef _MEMDEBUG
	g_MemoryGui.RunGui();
#endif // _MEMDEBUG


	ImGui::Begin("Particle Controll");
	ImGui::Checkbox("Create Particles", &createParticles);
	ImGui::Checkbox("Delete Particles", &deleteParticles);
	ImGui::InputInt("Amount", &amount);
	ImGui::Text("Num Particles: %d", g_ParticleEffect.GetNumParticles());
	ImGui::End();
	if (g_bUpdate)
	{
		g_ParticleEffect.Update(fDeltaTime, createParticles, deleteParticles, amount);
	}
	else
	{
		g_ParticleEffect.BuildVertexBuffer();
	}
	////new
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	//int display_w, display_h;
	//glfwGetFramebufferSize(window, &display_w, &display_h);
	////glViewport(0, 0, display_w, display_h);
	////glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	////glClear(GL_COLOR_BUFFER_BIT);
	//
	//ImGui::Render();
	//ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	////glfwSwapBuffers(window);

	ImGui::Render();
	int display_w, display_h;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);

	glutPostRedisplay();
}

template< typename T >
void Increment(T& value, const T& step, const T& max)
{
	value = std::min(max, value + step);
}

template< typename T >
void Decrement(T& value, const T& step, const T& min)
{
	value = std::max(min, value - step);
}

void SpecialKeysGL(int c, int x, int y)
{
	const float fStep = 1.0f;
	switch (c)
	{
	case GLUT_KEY_UP:
	{
		Increment(g_CubeEmitter.MaxWidth, fStep, 100.0f);
		Increment(g_CubeEmitter.MaxHeight, fStep, 100.0f);
		Increment(g_CubeEmitter.MaxDepth, fStep, 100.0f);
		g_CubeEmitter.MinWidth = -g_CubeEmitter.MaxWidth;
		g_CubeEmitter.MinHeight = -g_CubeEmitter.MaxHeight;
		g_CubeEmitter.MinDepth = -g_CubeEmitter.MaxDepth;

		Increment(g_ParticleEmitter.MinInclination, fStep, 180.0f);
		Decrement(g_ParticleEmitter.MaxInclination, fStep, g_ParticleEmitter.MinInclination);
	}
	break;
	case GLUT_KEY_DOWN:
	{
		Decrement(g_CubeEmitter.MaxWidth, fStep, 0.0f);
		Decrement(g_CubeEmitter.MaxHeight, fStep, 0.0f);
		Decrement(g_CubeEmitter.MaxDepth, fStep, 0.0f);

		g_CubeEmitter.MinWidth = -g_CubeEmitter.MaxWidth;
		g_CubeEmitter.MinHeight = -g_CubeEmitter.MaxHeight;
		g_CubeEmitter.MinDepth = -g_CubeEmitter.MaxDepth;

		Decrement(g_ParticleEmitter.MinInclination, fStep, 0.0f);
		Increment(g_ParticleEmitter.MaxInclination, fStep, 180.0f);
	}
	break;
	case GLUT_KEY_LEFT:
	{
		Increment(g_ParticleEmitter.MinAzimuth, fStep, 360.0f);
		Decrement(g_ParticleEmitter.MaxAzimuth, fStep, 0.0f);
	}
	break;
	case GLUT_KEY_RIGHT:
	{
		Decrement(g_ParticleEmitter.MinAzimuth, fStep, 0.0f);
		Increment(g_ParticleEmitter.MaxAzimuth, fStep, 360.0f);
	}
	break;
	};

	glutPostRedisplay();
}

void KeyboardGL(unsigned char c, int x, int y)
{
	float fRadiusStep = 1.0f;

	switch (c)
	{
	case ' ': // Space bar
	{
		// Toggle updating of the simulation
		g_bUpdate = !g_bUpdate;
	}
	break;
	case '-':
	{
		g_ParticleEmitter.MaximumRadius -= fRadiusStep;
		g_ParticleEmitter.MinimumRadius -= fRadiusStep;

		g_ParticleEmitter.MaximumRadius = std::max(0.0f, g_ParticleEmitter.MaximumRadius);
		g_ParticleEmitter.MaximumRadius = std::max(0.0f, g_ParticleEmitter.MaximumRadius);
	}
	break;
	case '+':
	{
		g_ParticleEmitter.MaximumRadius += fRadiusStep;
		g_ParticleEmitter.MinimumRadius += fRadiusStep;

		g_ParticleEmitter.MaximumRadius = std::min(200.0f, g_ParticleEmitter.MaximumRadius);
		g_ParticleEmitter.MaximumRadius = std::min(200.0f, g_ParticleEmitter.MaximumRadius);
	}
	break;
	case 's':
	case 'S':
	{
		std::cout << "Shade Model: GL_SMOOTH" << std::endl;
		// Switch to smooth shading model
		glShadeModel(GL_SMOOTH);
	}
	break;
	case 'f':
	case 'F':
	{
		std::cout << "Shade Model: GL_FLAT" << std::endl;
		// Switch to flat shading model
		glShadeModel(GL_FLAT);
	}
	break;
	case 'r':
	case 'R':
	{
		std::cout << "Reset Parameters..." << std::endl;
		g_Camera.SetTranslate(g_DefaultCameraTranslate);
		g_Camera.SetRotate(g_DefaultCameraRotate);
		g_Camera.SetPivot(g_DefaultCameraPivot);
	}
	break;
	case '\033': // escape quits
	case '\015': // Enter quits    
	case 'Q':    // Q quits
	case 'q':    // q (or escape) quits
	{
		// Cleanup up and quit
		exit(0);
	}
	break;
	}

	glutPostRedisplay();
}

void MouseGL(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		g_bLeftMouseDown = (state == GLUT_DOWN);
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		g_bRightMouseDown = (state == GLUT_DOWN);
	}
	g_MousePrevious = glm::vec2(x, y);

}

void MotionGL(int x, int y)
{
	g_MouseCurrent = glm::vec2(x, y);
	g_MouseDelta = (g_MousePrevious - g_MouseCurrent);

	// Update the camera
	if (g_bLeftMouseDown && g_bRightMouseDown)
	{
		g_Camera.TranslateZ(g_MouseDelta.y);
	}
	else if (g_bLeftMouseDown)
	{
		g_Camera.AddPitch(-g_MouseDelta.y);
		g_Camera.AddYaw(-g_MouseDelta.x);
	}
	else if (g_bRightMouseDown)
	{
		g_Camera.TranslatePivotX(g_MouseDelta.x);
		g_Camera.TranslatePivotY(-g_MouseDelta.y);
	}

	g_MousePrevious = g_MouseCurrent;
}

void ReshapeGL(int w, int h)
{
	std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

	h = std::max(h, 1);

	g_iWindowWidth = w;
	g_iWindowHeight = h;

	g_Camera.SetViewport(0, 0, w, h);
	g_Camera.ApplyViewport();

	g_Camera.SetProjection(60.0f, w / (float)h, 0.1f, 1000.0f);
	g_Camera.ApplyProjectionTransform();

	glutPostRedisplay();
}


#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "../include/u-gine.h"

#define FULLSCREEN false


int main(int, char**) {
	float rotatione = 0;

	// Creamos contexto de OpenGL (ventana o pantalla completa)
	if (FULLSCREEN)	Screen::Instance()->Open(Screen::Instance()->GetDesktopWidth(), Screen::Instance()->GetDesktopHeight(), true);
	else				Screen::Instance()->Open(1200, 900, false);


	//creamos el modelo para la malla y añadimos a escena
	/*std::shared_ptr<Model> Castillo = Model::Create(ResourceManager::Instance()->LoadMesh("data/simple-dm5.msh.xml"));
	Scene::Instance()->AddEntity(Castillo);

	//dotamos al castillo de posición rotación y escala
	Castillo->DefPosition() = glm::vec3(2, -1, 1);
	Castillo->DefRotation() = glm::quat(1,0,0,0);
	Castillo->DefScale() = glm::vec3(0.01f, 0.01f, 0.01f);*/

  //creamos el modelo para la malla y añadimos a escena
  std::shared_ptr<Model> Conejo = Model::Create(ResourceManager::Instance()->LoadMesh("data/skybox.msh.xml"));
  Scene::Instance()->AddEntity(Conejo);
  //dotamos al conejo de posición rotación y escala
  Conejo->DefPosition() = glm::vec3(0, 0, 0);
  Conejo->DefRotation() = glm::quat(1, 0, 0, 0);
  Conejo->DefScale() = glm::vec3(100,100,100);

  
  //creamos el modelo para la malla y añadimos a escena
  std::shared_ptr<Model> Conejo1 = Model::Create(ResourceManager::Instance()->LoadMesh("data/suzanne_refract.msh.xml"));
  Scene::Instance()->AddEntity(Conejo1);
  //dotamos al conejo de posición rotación y escala
  Conejo1->DefPosition() = glm::vec3(-3, 0, 5);
  Conejo1->DefRotation() = glm::quat(1, 0, 0, 0);
  Conejo1->DefScale() = glm::vec3(1, 1, 1);

  
  //creamos el modelo para la malla y añadimos a escena
  std::shared_ptr<Model> Conejo2 = Model::Create(ResourceManager::Instance()->LoadMesh("data/teapot_reflect.msh.xml"));
  Scene::Instance()->AddEntity(Conejo2);
  //dotamos al conejo de posición rotación y escala
  Conejo2->DefPosition() = glm::vec3(0, 0, 5);
  Conejo2->DefRotation() = glm::quat(1, 0, 0, 0);
  Conejo2->DefScale() = glm::vec3(1, 1, 1);
  

 //creamos el modelo para la malla y añadimos a escena
  std::shared_ptr<Model> Conejo3 = Model::Create(ResourceManager::Instance()->LoadMesh("data/cube.msh.xml"));
  Scene::Instance()->AddEntity(Conejo3);
  //dotamos al conejo de posición rotación y escala
  Conejo3->DefPosition() = glm::vec3(3, 0, 5);
  Conejo3->DefRotation() = glm::quat(1, 0, 0, 0);
  Conejo3->DefScale() = glm::vec3(1, 1, 1);
  

	//creamos la cámara y añadimos a escena
	std::shared_ptr<Camera> miCamara = Camera::Create();
	Scene::Instance()->AddEntity(miCamara);


  //creamos el modelo para la malla y añadimos a escena
  
  //creamos el modelo para la malla y añadimos a escena



  //creamos el modelo para la malla y añadimos a escena
 
  std::shared_ptr<Light>  pLight = Light::Create();
  Scene::Instance()->AddEntity(pLight);
  pLight->DefPosition() = glm::vec3(1, 1, 1);
  pLight->SetColor(glm::vec3(0.7, 0.7, 0.7));
  pLight->SetType(Light::Type::POINT);
  pLight->SetAttenuation(0.0f);


	/*std::shared_ptr<Light>  dLight = Light::Create();
	Scene::Instance()->AddEntity(dLight);
	dLight->DefPosition() = glm::vec3(1, 1, 1);
	dLight->SetColor(glm::vec3(0.7, 0.7, 0.7));
	dLight->SetType(Light::Type::DIRECTIONAL);
	dLight->SetAttenuation(1);*/

  Scene::Instance()->SetAmbient(glm::vec3(0, 0, 0));

	//establecemos parámetros de cámara:
	miCamara->SetColor(glm::vec3(0.4f, 0.4f, 0.6f));
	miCamara->SetUsesTarget(true);
	miCamara->SetViewport(0, 0, Screen::Instance()->GetWidth(), Screen::Instance()->GetHeight());
	miCamara->DefPosition()=glm::vec3(0, 0, 0);
	miCamara->DefRotation()=glm::quat(1, 0, 0, 0);

	// Establecemos la matriz de proyeccion de la cámara

	glm::mat4 proj = glm::perspective(glm::radians(60.0f), Screen::Instance()->GetWidth() * 1.0f / Screen::Instance()->GetHeight(), 0.1f, 3000.0f);
	miCamara->SetProjection(proj);
	
	glm::vec3 eye = glm::vec3(0, 0, 0);
	glm::vec3 forward = glm::vec3(0, 0, 1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 ctUp = glm::vec3(0, 1, 0);
	glm::vec3 side;

	glm::vec3 f2 = glm::vec3(0, 0, 1);

	miCamara->DefTarget()=eye+forward;

	glm::quat rueda, rueda1;
	uint32 k = 0;
	float yaw = 0, pitch = 0;
	float time;
	float movex=0.01;



	// Bucle principal. Se ejecuta mientras la ventana esté abierta y no pulsemos ESC
	while (!Screen::Instance()->ShouldClose() && !Screen::Instance()->IsKeyPressed(GLFW_KEY_ESCAPE)) {

		//shot is billboard
		

		time = Screen::Instance()->GetElapsed();
		pitch += (Screen::Instance()->GetMouseX() - Screen::Instance()->GetWidth() / 2)*Screen::Instance()->GetElapsed()*0.1f;
		yaw += (Screen::Instance()->GetMouseY() - Screen::Instance()->GetHeight() / 2)*Screen::Instance()->GetElapsed()*0.1f;

		if (yaw >= 1.57f) yaw = 1.57f;
		if (yaw <= -1.57f) yaw = -1.57f;

		rueda1 = glm::rotate(glm::quat(1,0,0,0), -pitch, glm::vec3(0, 1, 0));
		rueda = glm::conjugate(glm::rotate(glm::conjugate(rueda1), -yaw, rueda1*glm::vec3(1, 0, 0)));

		

		forward = glm::normalize(glm::vec3(rueda*glm::vec4(glm::vec3(0, 0, 1), 1)));
		up = glm::normalize(glm::vec3(rueda*glm::vec4(glm::vec3(0, 1, 0), 1)));
		side = glm::normalize(glm::cross(ctUp, forward));

		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_W))
		{
			eye += glm::normalize(glm::cross(side, ctUp))*time*1.0f;
		}
		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_S))
		{
			eye -= glm::normalize(glm::cross(side, ctUp))*time*1.0f;
		}
		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_A))
		{
			eye += side*time*1.0f;
		}
		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_D))
		{
			eye -= side*time*1.0f;
		}
		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_Q))
		{
			eye += ctUp*time*1.0f;
		}
		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_E))
		{
			eye -= ctUp*time*1.0f;
		}
    if (Screen::Instance()->IsKeyPressed(GLFW_KEY_P))
    {
      Conejo->TEST = 0.01f;
    }
    if (Screen::Instance()->IsKeyPressed(GLFW_KEY_O))
    {
      Conejo->TEST = -0.01f;
    }
    if (Screen::Instance()->IsKeyPressed(GLFW_KEY_K))
    {
      Conejo->TEST = -0.5f;
    }
    if (Screen::Instance()->IsKeyPressed(GLFW_KEY_L))
    {
      Conejo->TEST = +0.5f;
    }
		if (Screen::Instance()->IsMousePressed(0))
		{
		  pLight->DefPosition() = eye+0.2f*forward-0.1f*up;
	    //Faces->DefRotation() = rueda;
			f2 = forward;
		}

	//	Faces->DefRotation() = rueda*glm::quat(glm::cos(3.14 / 4), glm::sin(3.14 / 4), 0, 0)* glm::quat(glm::cos(-3.14 / 4*1.2), 0, 0, glm::sin(-3.14 / 4 * 1.2));

	//	Faces->DefPosition() = Faces->GetPosition()+f2*movex*10.0f;
		
		miCamara->DefRotation() = rueda;
		miCamara->DefPosition() = eye;
		miCamara->DefTarget() = eye + forward;
    pLight->DefPosition() = eye;
    Conejo->DefPosition() = eye;

		Scene::Instance()->Render();
    Scene::Instance()->Update(Screen::Instance()->GetElapsed());

		Screen::Instance()->MoveMouse(Screen::Instance()->GetWidth() / 2, Screen::Instance()->GetHeight() / 2);

		// Refrescamos la pantalla
		Screen::Instance()->Refresh();

		// Escribimos en el titulo de la ventana cualquier posible error que se pueda haber producido
		// al compilar los shaders
		Screen::Instance()->SetTitle(Renderer::Instance()->GetProgramError());

	}

	return 0;
}




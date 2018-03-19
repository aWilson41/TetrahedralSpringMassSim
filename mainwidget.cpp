#include "MainWidget.h"
#include "Engine\ResourceLoader.h"

#include <QMouseEvent>
#include <QApplication>
#include <qdesktopwidget.h>
#include <math.h>

MainWidget::MainWidget(QWidget* parent) : 
	QOpenGLWidget(parent)
{
	QRect rect = QApplication::desktop()->availableGeometry();
	setGeometry(150, 100, rect.width() * 1440.0 / 1920.0, rect.height() * 810.0 / 1080.0);

	// Update the mouse position to the current mouse position
	QPoint mousePt = mapFromGlobal(QCursor::pos());
	mousePos = glm::vec2(static_cast<GLfloat>(mousePt.x()), static_cast<GLfloat>(mousePt.y()));
	setMouseTracking(true);
	updateCamera(mousePos);
}

void MainWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.4f, 0.58f, 0.9f, 1.0f);
	initShaders();


	//tree = new SpringMassTree(5, 6);
	poly = resourceLoader::loadPolygon("C:/Users/Andx_/Desktop/test.obj");
	poly->setShaderProgram(&program);
	poly->world = mathHelper::matrixScale(0.2f);
	Material* treeMat = new Material();
	treeMat->setDiffuse(0.4f, 0.16f, 0.0f);
	treeMat->setAmbientToDiffuse(0.8f);
	materials.push_back(treeMat);
	poly->setMaterial(treeMat);

	plane = new Plane();
	plane->setShaderProgram(&program);
	plane->world = mathHelper::matrixTranslate(0.0f, -11.4f, 0.0f) * mathHelper::matrixScale(1000.0f);
	Material* planeMat = new Material();
	planeMat->setDiffuse(0.0f, 0.2f, 0.0f);
	planeMat->setAmbientToDiffuse(0.8f);
	materials.push_back(planeMat);
	plane->setMaterial(planeMat);

	lightDir = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));


	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	// Enable Multisampling
	glEnable(GL_MULTISAMPLE);

	// Use QBasicTimer because its faster than QTimer
	timer.start(12, this);
}

void MainWidget::initShaders()
{
	// Compile vertex shader
	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
		close();

	// Compile fragment shader
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
		close();

	// Link shader pipeline
	if (!program.link())
		close();

	// Bind shader pipeline for use
	if (!program.bind())
		close();
}

void MainWidget::mousePressEvent(QMouseEvent* e)
{

}

void MainWidget::mouseReleaseEvent(QMouseEvent* e)
{

}

void MainWidget::wheelEvent(QWheelEvent* e)
{
	rho -= e->delta() * 0.004f;
	updateCamera(mousePos);
}

void MainWidget::mouseMoveEvent(QMouseEvent* e)
{
	// Get the new mouse position as vec2
	glm::vec2 newMousePos = mathHelper::qPointToVec2(e->pos());
	if (e->buttons() == Qt::LeftButton)
		updateCamera(newMousePos);
	mousePos = newMousePos;
}

void MainWidget::updateCamera(glm::vec2 pos)
{
	glm::vec2 diff = mousePos - pos;
	theta -= diff.x * 0.008f;
	phi += diff.y * 0.008f;
	
	// Clamp
	if (phi > 3.14f)
		phi = 3.14f;
	else if (phi < 0.01f)
		phi = 0.01f;

	// Convert spherical coords
	glm::vec3 eyePos = glm::vec3(
		rho * sin(phi) * cos(theta),
		rho * cos(phi),
		rho * sin(phi) * sin(theta));

	// Should prob use a slerp here but ohwell
	cam.setEyePos(eyePos);
	cam.setFocalPt(0.0f, 0.0f, 0.0f);
	cam.updateLookAt();
}

void MainWidget::timerEvent(QTimerEvent* e)
{
	update();
}

void MainWidget::resizeGL(int w, int h)
{
	// Calculate aspect ratio
	qreal aspect = qreal(w) / qreal(h ? h : 1);

	// Set near plane to 0.001, far plane to 700.0, field of view 45 degrees
	// Set the perspective projection matrix
	cam.setPerspective(45.0, aspect, 0.001f, 1000.0f);
}

void MainWidget::paintGL()
{
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.bind();

	// Set modelview-projection matrix
	glm::mat4 viewProj = cam.proj * cam.view;
	glUniform3f(program.uniformLocation("lightDir"), lightDir.x, lightDir.y, lightDir.z);
	poly->draw(viewProj);
	plane->draw(viewProj);

	program.release();
}

MainWidget::~MainWidget()
{
	for (UINT i = 0; i < materials.size(); i++)
	{
		if (materials[i] != nullptr)
			delete materials[i];
	}

	if (poly != nullptr)
		delete poly;
	if (plane != nullptr)
		delete plane;

	// Make sure the context is current when deleting the texture and the buffers.
	makeCurrent();
	//delete tree;
	doneCurrent();
}
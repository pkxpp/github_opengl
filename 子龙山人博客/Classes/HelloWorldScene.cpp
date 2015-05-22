#include "HelloWorldScene.h"

USING_NS_CC;

GLuint vao;
GLuint vertexVBO;
GLuint colorVBO;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //opengl study
    // 1
	//this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));

	// 2. shader
	//create my own program
	mShaderProgram = new GLProgram;
	mShaderProgram->initWithFilenames("myVertexShader.vert", "myFragmentShader.frag");
	mShaderProgram->link();
	//set uniform locations
	mShaderProgram->updateUniforms();

	// 3.
	this->setGLProgram(mShaderProgram);

    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::visit(Renderer *renderer, const Mat4 &transform, uint32_t transformUpdated)
{
    Layer::visit(renderer, transform, transformUpdated);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void HelloWorld::onDraw()
{
	/*
    //获得当前HelloWorld的shader
    //auto glProgram = getGLProgram;			// 1
	auto glProgram = mShaderProgram;		// 2

   //使用此shader
    glProgram->use();
   //设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
    glProgram->setUniformsForBuiltins();

    auto size = Director::getInstance()->getWinSize();
    //指定将要绘制的三角形的三个顶点，分别位到屏幕左下角，右下角和正中间的顶端
    float vertercies[] = { 0,0,   //第一个点的坐标
                            size.width, 0,   //第二个点的坐标
                           size.width / 2, size.height};  //第三个点的坐标
    //指定每一个顶点的颜色，颜色值是RGBA格式的，取值范围是0-1
    float color[] = { 0, 1,0, 1,    //第一个点的颜色，绿色
                        1,0,0, 1,  //第二个点的颜色, 红色
                         0, 0, 1, 1};  //第三个点的颜色， 蓝色
    //激活名字为position和color的vertex attribute
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
    //分别给position和color指定数据源
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
    //绘制三角形，所谓的draw call就是指这个函数调用
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //通知cocos2d-x 的renderer，让它在合适的时候调用这些OpenGL命令
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
    //如果出错了，可以使用这个函数来获取出错信息
    CHECK_GL_ERROR_DEBUG();*/

	/***********************2*************************/
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

    //获得当前HelloWorld的shader
    //auto glProgram = getGLProgram;			// 1
	auto glProgram = mShaderProgram;		// 2

   //使用此shader
    glProgram->use();
   //设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
    glProgram->setUniformsForBuiltins();

	//创建和绑定vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    auto size = Director::getInstance()->getWinSize();
  
	//新的数据结构
	Vertex data[] = {
		{{-1, -1}, {0, 1, 0, 1}},
		{{1, -1}, {0, 1, 0, 1}},
		{{-1, 1}, {0, 1, 0, 1}},
		{{1, 1}, {0, 1, 0, 1}},
	};

	GLubyte indices[] = {
		0, 1, 2,			// 第一个三角形索引
		2, 3, 1,			// 第二个三角形索引
	};

	//创建和绑定vbo
	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	//获取vertex attribute 'a_position'入口点
	GLuint positionLocation = glGetAttribLocation(glProgram->getProgram(), "a_position");
	//打开a_position入口点
	glEnableVertexAttribArray(positionLocation);
	//传递定点数据个a_position，注意最后一个参数是数组的便宜了
	glVertexAttribPointer(positionLocation, 2, 
						GL_FLOAT, GL_FALSE, 
						sizeof(Vertex), 
						(GLvoid*)offsetof(Vertex, Position));

	//set for color
	//glGenBuffers(1, &colorVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GLuint colorLocation = glGetAttribLocation(glProgram->getProgram(), "a_color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(GLvoid*)offsetof(Vertex, Color));

	GLuint indexVbo;
	glGenBuffers(1, &indexVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);

    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
    CHECK_GL_ERROR_DEBUG();

	//
	GLuint uColorLocation = glGetUniformLocation(glProgram->getProgram(), "u_color");
	float uColor[] = {1.0, 1.0, 1.0, 1.0, 1.0};
	glUniform4fv(uColorLocation, 1, uColor);

	//for safty
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
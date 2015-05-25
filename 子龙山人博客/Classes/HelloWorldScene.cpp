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
    //��õ�ǰHelloWorld��shader
    //auto glProgram = getGLProgram;			// 1
	auto glProgram = mShaderProgram;		// 2

   //ʹ�ô�shader
    glProgram->use();
   //���ø�shader��һЩ����uniform,��Ҫ��MVP����model-view-project����
    glProgram->setUniformsForBuiltins();

    auto size = Director::getInstance()->getWinSize();
    //ָ����Ҫ���Ƶ������ε��������㣬�ֱ�λ����Ļ���½ǣ����½Ǻ����м�Ķ���
    float vertercies[] = { 0,0,   //��һ���������
                            size.width, 0,   //�ڶ����������
                           size.width / 2, size.height};  //�������������
    //ָ��ÿһ���������ɫ����ɫֵ��RGBA��ʽ�ģ�ȡֵ��Χ��0-1
    float color[] = { 0, 1,0, 1,    //��һ�������ɫ����ɫ
                        1,0,0, 1,  //�ڶ��������ɫ, ��ɫ
                         0, 0, 1, 1};  //�����������ɫ�� ��ɫ
    //��������Ϊposition��color��vertex attribute
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
    //�ֱ��position��colorָ������Դ
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
    //���������Σ���ν��draw call����ָ�����������
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //֪ͨcocos2d-x ��renderer�������ں��ʵ�ʱ�������ЩOpenGL����
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
    //��������ˣ�����ʹ�������������ȡ������Ϣ
    CHECK_GL_ERROR_DEBUG();*/

	/***********************2*************************/
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

	Mat4 modelViewMatrix;
    Mat4::createLookAt(Vec3(0,0,1), Vec3(0,0,0), Vec3(0,1,0), &modelViewMatrix);
    
    modelViewMatrix.translate(0, 0, -5);

    static float rotation = 0;
    modelViewMatrix.rotate(Vec3(1,1,1),CC_DEGREES_TO_RADIANS(rotation));
    rotation++;
    if (rotation > 360) {
        rotation = 0;
    }

    
    Mat4 projectionMatrix;
    Mat4::createPerspective(60, 480/320, 1.0, 42, &projectionMatrix);
    Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, projectionMatrix);
    
    
    Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelViewMatrix);


    //��õ�ǰHelloWorld��shader
    //auto glProgram = getGLProgram;			// 1
	auto glProgram = mShaderProgram;		// 2

   //ʹ�ô�shader
    glProgram->use();
   //���ø�shader��һЩ����uniform,��Ҫ��MVP����model-view-project����
    glProgram->setUniformsForBuiltins();

	//�����Ͱ�vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    auto size = Director::getInstance()->getWinSize();
  
	//�µ����ݽṹ
	Vertex data[] =
    {
        // Front
        { {1, -1, 0}, {1, 0, 0, 1}},
        { {1, 1, 0}, {0, 1, 0, 1}},
        { {-1, 1, 0}, {0, 0, 1, 1}},
        { {-1, -1, 0}, {0, 0, 0, 1}},
        // Back
        { {1, 1, -2}, {1, 0, 0, 1}},
        { {-1, -1, -2}, {0, 1, 0, 1}},
        { {1, -1, -2}, {0, 0, 1, 1}},
        { {-1, 1, -2}, {0, 0, 0, 1}},
        // Left
        { {-1, -1, 0}, {1, 0, 0, 1}},
        { {-1, 1, 0}, {0, 1, 0, 1}},
        { {-1, 1, -2}, {0, 0, 1, 1}},
        { {-1, -1, -2}, {0, 0, 0, 1}},
        // Right
        { {1, -1, -2}, {1, 0, 0, 1}},
        { {1, 1, -2}, {0, 1, 0, 1}},
        { {1, 1, 0}, {0, 0, 1, 1}},
        { {1, -1, 0}, {0, 0, 0, 1}},
        // Top
        { {1, 1, 0}, {1, 0, 0, 1}},
        { {1, 1, -2}, {0, 1, 0, 1}},
        { {-1, 1, -2}, {0, 0, 1, 1}},
        { {-1, 1, 0}, {0, 0, 0, 1}},
        // Bottom
        { {1, -1, -2}, {1, 0, 0, 1}},
        { {1, -1, 0}, {0, 1, 0, 1}},
        { {-1, -1, 0}, {0, 0, 1, 1}},
        { {-1, -1, -2}, {0, 0, 0, 1}}
    };

	//GLubyte indices[] = {
	//	0, 1, 2,			// ��һ������������
	//	2, 3, 1,			// �ڶ�������������
	//};

	GLubyte indices[] = {
        // Front
        0, 1, 2,
        2, 3, 0,
        // Back
        4, 5, 6,
        4, 5, 7,
        // Left
        8, 9, 10,
        10, 11, 8,
        // Right
        12, 13, 14,
        14, 15, 12,
        // Top
        16, 17, 18,
        18, 19, 16,
        // Bottom
        20, 21, 22,
        22, 23, 20
    };

	//�����Ͱ�vbo
	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	//��ȡvertex attribute 'a_position'��ڵ�
	GLuint positionLocation = glGetAttribLocation(glProgram->getProgram(), "a_position");
	//��a_position��ڵ�
	glEnableVertexAttribArray(positionLocation);
	//���ݶ������ݸ�a_position��ע�����һ������������ı�����
	glVertexAttribPointer(positionLocation, 
						3, 
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
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);

    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 36);
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
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
	//创建和绑定vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	auto size = Director::getInstance()->getWinSize();

	//新的数据结构
	/*Vertex data[] =
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
    };*/

	Vertex data[] =
    {
        { {-1,-1},{0,1,0,1},{0,1}},
        { {1,-1},{0,1,0,1},{1,1}},
        { {-1,1},{0,1,0,1},{0,0}},
        { {1,1},{0,1,0,1},{1,0}}
    };

	GLubyte indices[] = {
		0, 1, 2,			// 第一个三角形索引
		2, 3, 1,			// 第二个三角形索引
	};

	/*GLubyte indices[] = {
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
    };*/

	//创建和绑定vbo
	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	//获取vertex attribute 'a_position'入口点
	GLuint positionLocation = glGetAttribLocation(mShaderProgram->getProgram(), "a_position");
	//打开a_position入口点
	glEnableVertexAttribArray(positionLocation);
	//传递定点数据个a_position，注意最后一个参数是数组的便宜了
	glVertexAttribPointer(positionLocation, 
						2, 
						GL_FLOAT, GL_FALSE, 
						sizeof(Vertex), 
						(GLvoid*)offsetof(Vertex, Position));

	//set for color
	//glGenBuffers(1, &colorVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GLuint colorLocation = glGetAttribLocation(mShaderProgram->getProgram(), "a_color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(GLvoid*)offsetof(Vertex, Color));

	GLuint textureLocation = glGetAttribLocation(mShaderProgram->getProgram(), "a_coord");
    glEnableVertexAttribArray(textureLocation);
    glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));

	GLuint indexVbo;
	glGenBuffers(1, &indexVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//method 1: the hard way
    Image *image = new Image;
    std::string imagePath = FileUtils::getInstance()->fullPathForFilename("HelloWorld.png");
    image->initWithImageFile(imagePath);
    
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 8);

	GLuint textureId;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
//    GL::bindTexture2DN(1,textureId);



    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    
    unsigned char *imageData = image->getData();
    int width = image->getWidth();
    int height = image->getHeight();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 width,
                 height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,//must be GL_UNSIGNED_BYTE
                 imageData);
    
    
//    //method2: the easier way
//    Texture2D *texture = new Texture2D;
//    texture->initWithImage(image);
//    textureId = texture->getName();
//    
//    //method3: the easiest way
//    Sprite *sprite = Sprite::create("HelloWorld.png");
//    textureId = sprite->getTexture()->getName();
    
    CC_SAFE_DELETE(image);

	//for safty
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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


    //获得当前HelloWorld的shader
    //auto glProgram = getGLProgram;			// 1
	auto glProgram = mShaderProgram;		// 2

   //使用此shader
    glProgram->use();
   //设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
    glProgram->setUniformsForBuiltins();

    GLuint textureLocation = glGetUniformLocation(glProgram->getProgram(),"CC_Texture0");
    glUniform1i(textureLocation, 0);
	
	//
	GLuint uColorLocation = glGetUniformLocation(glProgram->getProgram(), "u_color");
	float uColor[] = {1.0, 1.0, 1.0, 1.0, 1.0};
	glUniform4fv(uColorLocation, 1, uColor);

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);
	 glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE,(GLvoid*)0);

    //CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 36);
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
    CHECK_GL_ERROR_DEBUG();

	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
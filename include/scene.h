#ifndef _scene
#define _scene

class Scene
{
public:
	virtual void update() = 0;
	virtual void render() = 0;

	virtual void reshape( int w, int h ) = 0;
	virtual void mouse( int button, int state, int x, int y ) = 0;
	virtual void motion( int x, int y ) = 0;
	virtual void passive_motion( int x, int y ) = 0;
	virtual void keyboard( unsigned char key, int x, int y ) = 0;
	virtual void keyboardup( unsigned char key, int x, int y ) = 0;
	virtual void special_keyboard( int key, int x, int y ) = 0;

	virtual ~Scene() {}
};

#endif
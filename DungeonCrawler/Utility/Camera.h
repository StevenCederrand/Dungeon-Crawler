#ifndef _CAMERA_H
#define _CAMERA_H
#include <GLM/glm.hpp>
#include "Ray.h"

class Camera {

public:
	static Camera* active;

	const float NEAR_CLIPPING = 0.1f;
	const float FAR_CLIPPING = 70.f;
	const float FOV = 70.f;
public:
	Camera();
	~Camera();

	void update(float dt);

	void setToOrbit(bool shouldOrbit);
	const glm::mat4 getViewMatrix() const;
	const glm::mat4 getProjectionMatrix() const;
	const glm::vec3 getPosition() const;
	const Ray getRayFromScreen(float x, float y, float w, float h) const;
	const glm::vec3 getMouseWorldPos();

	void setToPlayer(glm::vec3 playerPos, glm::vec3 shakeDir);
	void setAngle(int angle);

private:
	void setProjectionMatrix();
	void setViewMatrix();
	void calculateCameraAxis();

	void lookAround(float dt);
	void move(float dt);
	void snapMouseToMiddle();

	void setDistanceToOrbitPoint(float distance);

private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	glm::dvec2 m_mousePos;

	glm::vec3 m_position;
	glm::vec3 m_cameraUp;
	glm::vec3 m_cameraRight;
	glm::vec3 m_lookDirection;

	// For free camera look
	glm::dvec2 m_lastMousePosition;
	glm::dvec2 m_currentMousePosition;
	float m_sensitivity;
	float m_cameraSpeed;
	//______________________

	float m_yaw;
	float m_pitch;
	float m_distanceToOrbitPoint;


	// Free look or is it stuck on player?
	// (Debug purpose)
	bool m_locked;
	bool m_debug;
	int m_angle;
};


#endif

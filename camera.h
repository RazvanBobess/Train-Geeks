#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace camera {
    class Camera {
        public: 
            Camera() {
                position = glm::vec3(0, 2, 5);
                forward = glm::vec3(0, 0, -1);
                right = glm::vec3(1, 0, 0);
                up = glm::vec3(0, 1, 0);
                distanceToTarget = 2;
            }

            Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up) {
                Set(position, center, up);
            };

            ~Camera() { }

            void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up) {
                this->position = position;
                forward = glm::normalize(center - position);
                right = glm::cross(forward, up);
                this->up = glm::cross(right, forward);
            }

            void MoveForward(float distance) {
                glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
                position += dir * distance;
            }

            void TranslateForward(float distance) {
                glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
                position += dir * distance;
            }

            void TranslateUpward(float distance) {
                glm::vec3 dir = glm::normalize(glm::vec3(0, up.y, 0));
                position += dir * distance;
            }

            void TranslateRight(float distance) {
                glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
                position += dir * distance;
            }

            void RotateFirstPerson_OX(float angle) {
                glm::vec4 newVect = glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 1);
                forward = glm::normalize(glm::vec3(newVect));

                up = glm::cross(right, forward);
            }

            void RotateFirstPerson_OY(float angle) {
                glm::vec4 newVect = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
                forward = glm::normalize(glm::vec3(newVect));

                glm::vec4 rightVect = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
                right = glm::normalize(glm::vec3(rightVect));

                up = glm::cross(right, forward);
            }

            void RotateFirstPerson_OZ(float angle) {
                glm::vec4 newVect = glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(right, 1);
                right = glm::normalize(glm::vec3(newVect));
                
                up = glm::cross(right, forward);
            }

            glm::mat4 GetViewMatrix() {
                return glm::lookAt(position, position + forward, up);
            }

            void RotateThirdPerson_OX(float angle) {
                TranslateRight(distanceToTarget);
                RotateFirstPerson_OX(angle);
                TranslateRight(-distanceToTarget);
            }

            void RotateThirdPerson_OY(float angle) {
                TranslateUpward(distanceToTarget);
                RotateFirstPerson_OY(angle);
                TranslateUpward(-distanceToTarget);
            }

            void RotateThirdPerson_OZ(float angle) {
                TranslateForward(distanceToTarget);
                RotateFirstPerson_OZ(angle);
                TranslateForward(-distanceToTarget);
            }

            glm::vec3 GetTargetPosition() {
                return position + forward * distanceToTarget;
            }

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}
#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 right, glm::vec3 scale, glm::vec3 intensity, glm::vec3 ambientIntensity)
    : Transform(position, front, up, right, scale), intensity(intensity), ambientIntensity(ambientIntensity) {}

#version 460 core

uniform vec3 planetColor;
uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler3D sphereVolume;

uniform int windowHeight;
uniform int windowWidth;

in vec3 fragPos;

out vec4 FragColor;

// Fast Voxel Traversal from Woo and Amanatides
int traverse(vec3 rayOrigin, vec3 rayDir) {
	int stepX = int(sign(rayDir.x));
	int stepY = int(sign(rayDir.y));
	int stepZ = int(sign(rayDir.z));

	int x = int(floor(rayOrigin.x));
	int y = int(floor(rayOrigin.y));
	int z = int(floor(rayOrigin.z));

	float tDeltaX = stepX / rayDir.x;
	float tDeltaY = stepY / rayDir.y;
	float tDeltaZ = stepZ / rayDir.z;

	float tMaxX = stepX > 0 ? (x + 1 - rayOrigin.x) / rayDir.x : (rayOrigin.x - x) / rayDir.x;
	float tMaxY = stepY > 0 ? (y + 1 - rayOrigin.y) / rayDir.y : (rayOrigin.y - y) / rayDir.y;
	float tMaxZ = stepZ > 0 ? (z + 1 - rayOrigin.z) / rayDir.z : (rayOrigin.z - z) / rayDir.z;

	ivec3 textureDim = textureSize(sphereVolume, 0);

	while (x >= 0 && y >= 0 && z >= 0 && x < textureDim.x && y < textureDim.y && z < textureDim.z) {
		if (tMaxX < tMaxY) {
		  if (tMaxX < tMaxZ) {
			x += stepX;
			tMaxX += tDeltaX;
		  } else {
			z += stepZ;
			tMaxZ += tDeltaZ;
		  }
		} else {
		  if (tMaxY < tMaxZ) {
			y += stepY;
			tMaxY += tDeltaY;
		  } else {
			z += stepZ;
			tMaxZ += tDeltaZ;
		  }
		} 
		vec3 texCoord = (vec3(x,y,z) + 0.5) / vec3(textureDim);
		int textureValue = int(texture(sphereVolume, texCoord).r);

		if (textureValue > 0) {
			return textureValue;
		}
	}
	return 0;
}


void main()
{
	vec3 rayOrigin = vec3(fragPos);
	vec3 rayDir = normalize(rayOrigin - viewPos);
	FragColor = vec4(1.0, 0, 0, 1.0)  * traverse(rayOrigin, rayDir);
}
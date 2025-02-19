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
float traverse(vec3 rayOrigin, vec3 rayDir) {
	int x = int(floor(rayOrigin.x));
	int y = int(floor(rayOrigin.y));
	int z = int(floor(rayOrigin.z));	
	
	int stepX = int(sign(rayDir.x));
	int stepY = int(sign(rayDir.y));
	int stepZ = int(sign(rayDir.z));

	float tDeltaX = stepX / rayDir.x;
	float tDeltaY = stepY / rayDir.y;
	float tDeltaZ = stepZ / rayDir.z;

	float tMaxX = stepX > 0 ? (x + 1 - rayOrigin.x) / rayDir.x : (rayOrigin.x - x) / rayDir.x;
	float tMaxY = stepY > 0 ? (y + 1 - rayOrigin.y) / rayDir.y : (rayOrigin.y - y) / rayDir.y;
	float tMaxZ = stepZ > 0 ? (z + 1 - rayOrigin.z) / rayDir.z : (rayOrigin.z - z) / rayDir.z;

	ivec3 textureDim = textureSize(sphereVolume, 0);

	while (x >= 0 && y >= 0 && z >= 0 && x <= textureDim.x && y <= textureDim.y && z <= textureDim.z) {
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
		vec3 texCoord = vec3(x,y,z) / vec3(textureDim + 1);
		int textureValue = int(texture(sphereVolume, texCoord).r);

		if (textureValue > 0) {
			return textureValue;
		}
	}
	return 0;
}


void main()
{
	vec2 ndc = (gl_FragCoord.xy / vec2(windowWidth, windowHeight)) * 2.0 - 1.0;
	vec4 pixelPos = inverse(projection) * vec4(ndc, 1.0, 1.0);
	pixelPos /= pixelPos.w;
	vec3 rayOrigin = vec3(inverse(model) * vec4(fragPos, 1.0));
	vec3 rayDir = normalize(vec3(inverse(view) * pixelPos) - viewPos);
	FragColor = vec4(rayOrigin / textureSize(sphereVolume, 0), 1.0)  * traverse(rayOrigin, rayDir);
}
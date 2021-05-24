#pragma once

static const char* s_litTexturedFragmentPtr = R"(
    #version 330 core
	in vec3 FragPos;
	in vec3 Normal;
	in vec2 TexCoords;
    in vec4 FragPosLightSpace;

	out vec4 FragColor;

	struct Material {
		// We're skipping ambient, because it's usually the same as the light's color.
        sampler2D diffuse;
        sampler2D specular;
        float     shininess;
    };  
	struct DirLight {
        vec3 direction;		// The direction of the "sunlight" rays.

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };
	struct PointLight {    
        vec3 position;

        float constant;
        float linear;
        float quadratic;  

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };
	struct SpotLight {
		vec3 position;
		vec3 direction;

		float cutOff;	// The cosine of the intersection angle (the cone of the reflector) - the cosine is here to make the calculations easier, because in fact we later calculate the cosine of theta angle, not the angle itself.

		vec3 ambient;
        vec3 diffuse;
        vec3 specular;
	};

	uniform bool isHighlighted;
	uniform vec3 viewPos; // Viewer's (player's) position.
	uniform vec3 lightPos;
	uniform sampler2D shadowMap;
	uniform Material material;
	uniform DirLight dirLight;
	uniform PointLight pointLight;
	uniform SpotLight spotLight1;
	uniform SpotLight spotLight2;

	vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
	vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
	vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
	float ShadowCalculation(vec4 fragPosLightSpace, float bias);



	void main()
	{
		vec3 result = vec3(0.0);
		vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(viewPos - FragPos);

		// DIRECTIONAL
		result += CalcDirLight(dirLight, norm, viewDir);

		// POINT
		result += CalcPointLight(pointLight, norm, FragPos, viewDir);

		// SPOT
		result += CalcSpotLight(spotLight1, norm, FragPos, viewDir);
		result += CalcSpotLight(spotLight2, norm, FragPos, viewDir);
		
		if(isHighlighted)
			result += vec3(0.3);

		FragColor = vec4(result, 1.0);
	}



	vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
    {
        vec3 lightDir = normalize(-light.direction); // Our ray should shine TOWARDS the light source to make the further calculations correct, hence the negation.

        // diffuse light
        float diff = max(dot(normal, lightDir), 0.0);

        // specular light
        //vec3 reflectDir = reflect(-lightDir, normal); // Reflect function expects that the first vector points FROM the light source towards the fragment's position.
        //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		// PHONG -> BLINN-PHONG
		vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 2.0);

        // merging
        vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
        vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

		// calculating shadow
		float shadow = ShadowCalculation(FragPosLightSpace, max(0.05 * (1.0 - dot(normal, lightDir)), 0.005));

        return (ambient + (1.0 - shadow) * (diffuse + specular));
    }

	vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
	{
		vec3 lightDir = normalize(light.position - fragPos);

        // diffuse light
        float diff = max(dot(normal, lightDir), 0.0);

        // specular light
        //vec3 reflectDir = reflect(-lightDir, normal);  // Reflect function expects that the first vector points FROM the light source towards the fragment's position.
        //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		// PHONG -> BLINN-PHONG
		vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 2.0);

        // attenuation
        float distance    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
      			            light.quadratic * (distance * distance));    
        // merging
        vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
        vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        return (ambient + diffuse + specular);
	}

	vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
	{
		vec3 lightDir = normalize(light.position - fragPos);
		float theta = dot(lightDir, normalize(-light.direction)); // Negation, because we want the vector to point TO the light source.

		vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));

		if(theta > light.cutOff) // > instead of < is caused by the behavior of the cosine function.
		{
			// diffuse light
	        float diff = max(dot(normal, lightDir), 0.0);

	        // specular light
	        //vec3 reflectDir = reflect(-lightDir, normal);  // Reflect function expects that the first vector points FROM the light source towards the fragment's position.
	        //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
			// PHONG -> BLINN-PHONG
			vec3 halfwayDir = normalize(lightDir + viewDir);  
			float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 2.0);

			vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
			vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

			return (ambient + diffuse + specular);
		}
		else
		{
			return (ambient);
		}
	}

	float ShadowCalculation(vec4 fragPosLightSpace, float bias)
	{
		// perspective
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        // normalization
        projCoords = projCoords * 0.5 + 0.5;
        // getting nearest depth value from light's perspective
        float closestDepth = texture(shadowMap, projCoords.xy).r; 
        // getting current depth value from light's perspective
        float currentDepth = projCoords.z;
        // checking if current fragment (pixel) is in shadow
        float shadow = 0.0;
	    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	    for(int x = -3; x <= 3; ++x)
	    {
	        for(int y = -3; y <= 3; ++y)
	        {
	            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
	            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
	        }    
	    }
	    shadow /= 9.0 * 9.0;
	    
	    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	    if(projCoords.z > 1.0)
	        shadow = 0.0;
	        
	    return shadow;
	}
)";

#pragma once

static const char* s_litTexturedFragmentPtr = R"(
    #version 330 core
	in vec2 TexCoords;
	in vec3 Normal;
	in vec3 FragPos;

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

	uniform vec3 viewPos; // Viewer's (player's) position.
	uniform Material material;
	uniform DirLight dirLight;
	uniform PointLight pointLight;
	uniform SpotLight spotLight1;
	uniform SpotLight spotLight2;

	vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
	vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
	vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);



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

        return (ambient + diffuse + specular);
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
)";

#pragma once

static const char* LitTexturedF = R"(
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
        vec3 direction;		// Kierunek padania promieni swiatla "slonecznego".

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
		vec3 position;		// Pozycja reflektora.
		vec3 direction;		// Kierunek reflektora.

		float cutOff;		// Cosinus kata odciecia (stozka reflektora) - cosinus jest dla ulatwienia obliczen, bo tak naprawde obliczamy pozniej cosinus kata theta, a nie sam ten kat.

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
        vec3 lightDir = normalize(-light.direction); // Nasz promien powinien swiecic w kierunku DO zrodla swiatla, zeby dalsze obliczenia byly poprawne, stad negacja.

        // diffuse light
        float diff = max(dot(normal, lightDir), 0.0);

        // specular light
        //vec3 reflectDir = reflect(-lightDir, normal); // Funkcja reflect oczekuje, ze pierwszy wektor wskazuje OD zrodla swiatla w kierunku polozenia fragmentu.
        //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		// ZMIANA PHONG -> BLINN-PHONG
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
        //vec3 reflectDir = reflect(-lightDir, normal);  // Funkcja reflect oczekuje, ze pierwszy wektor wskazuje OD zrodla swiatla w kierunku polozenia fragmentu.
        //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		// ZMIANA PHONG -> BLINN-PHONG
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
		float theta = dot(lightDir, normalize(-light.direction)); // Negacja, bo chcemy zeby wektor byl skierowany DO zrodla swiatla.

		vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));

		if(theta > light.cutOff) // Wiekszosc zamiast mniejszosci wynika z zachowania funkcji cosinus.
		{
			// diffuse light
	        float diff = max(dot(normal, lightDir), 0.0);

	        // specular light
	        //vec3 reflectDir = reflect(-lightDir, normal);  // Funkcja reflect oczekuje, ze pierwszy wektor wskazuje OD zrodla swiatla w kierunku polozenia fragmentu.
	        //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
			// ZMIANA PHONG -> BLINN-PHONG
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

#version 330 core	     // Minimal GL version support expected from the GPU

uniform vec3 camPos;
in vec3 fPosition;
in vec3 fNormal;
out vec4 color;	  // Shader output: the color response attached to this fragment

//uniform sunPos; //TODO : Déterminer sunPos
uniform vec3 planetColor;


uniform int isLighted;



void main() {
	vec3 sunPos = vec3(0.0f); //A enlever qd sera importé

	vec3 lightColor = vec3(1.0);

	vec3 n = normalize(fNormal);
	vec3 l = normalize(sunPos-fPosition); // light direction vector
	
	vec3 v = normalize(camPos - fPosition); // view vector
	vec3 r = normalize(reflect(-l,n)); //reflection vector
	
	vec3 ambient = vec3(0.2); //ambient color

	//variables
	vec3 diffLightColor = lightColor;
	vec3 diffSurfaceColor = planetColor; 
	float kd = 1; //diffuse reflection constant

	vec3 diffuse = kd*max(dot(n,l),0)*diffLightColor*diffSurfaceColor; //diffuse lighting
	
	//variables
	float ks = 1; //specular reflection constant
	float alpha = 64; //shininess
	vec3 specSurfaceColor = vec3(0.8);
	vec3 specLightColor = lightColor;

	vec3 specular = ks*pow(max(dot(v,r),0), alpha)*specSurfaceColor*specLightColor; // specular lighting

	if(isLighted!=0) {
		color = vec4(ambient + diffuse + specular, 1.0); // Building RGBA from RGB.
	}
	else {
		color = vec4(planetColor, 1.0);
	}
}

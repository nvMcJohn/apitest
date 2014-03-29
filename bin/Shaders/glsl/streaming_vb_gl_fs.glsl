// NOTE: Unlike every other set of Solutions, all of the StreamingVBs currently use this shader.
// If you need to make modifications for a particular test, create a new shader. 
#version 410

// Uniforms / SSBO ----------------------------------------------------------------------------------------------------

// Input --------------------------------------------------------------------------------------------------------------

// Output -------------------------------------------------------------------------------------------------------------
layout(location = 0) out vec4 FS_Col;

// Functions ----------------------------------------------------------------------------------------------------------
void main()
{
	FS_Col = vec4(1, 1, 1, 1);
}

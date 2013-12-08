const char s_vs_code[] = 
    "#version 430\n"
    "\n"
    "layout(std140, binding = 0) uniform CB0\n"
    "{\n"
    "\tvec2 Viewport;\n"
    "};\n"
    "layout (location = 0) in vec2 Attr_Pos;\n"
    "void main()\n"
    "{\n"
    "\tgl_Position = vec4(Attr_Pos * Viewport + vec2(-1, 1), 0, 1);\n"
    "}\n"
    "";

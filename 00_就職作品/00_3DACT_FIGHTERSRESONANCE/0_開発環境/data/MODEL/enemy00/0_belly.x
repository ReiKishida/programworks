xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 263;
 5.20640;-1.88503;2.00755;,
 5.50388;-1.88503;-0.00567;,
 4.99637;-0.60278;-0.00554;,
 4.21094;-1.88503;3.86165;,
 5.20640;-1.88503;2.00755;,
 4.72529;-0.60278;2.01063;,
 0.41839;-0.60277;-4.83626;,
 0.48002;-1.88503;-4.82934;,
 -0.21698;-1.88503;-4.82934;,
 3.79494;-0.60277;-4.03125;,
 4.18543;-1.88503;-4.02551;,
 2.22053;-1.88503;-4.51942;,
 -0.21699;-1.88503;4.47729;,
 0.48001;-1.88503;4.44114;,
 0.41838;-0.60278;4.44778;,
 2.00443;-0.60277;-4.52589;,
 2.22053;-1.88503;-4.51942;,
 0.48002;-1.88503;-4.82934;,
 2.22053;-1.88503;4.27161;,
 4.21094;-1.88503;3.86165;,
 3.81818;-0.60278;3.86745;,
 4.72529;-0.60277;-2.02171;,
 5.20640;-1.88503;-2.01890;,
 4.18543;-1.88503;-4.02551;,
 0.48001;-1.88503;4.44114;,
 2.22053;-1.88503;4.27161;,
 2.00443;-0.60278;4.27801;,
 4.99637;-0.60278;-0.00554;,
 5.50388;-1.88503;-0.00567;,
 5.20640;-1.88503;-2.01890;,
 0.41839;-0.60277;-4.83626;,
 -0.21698;-0.60277;-4.83626;,
 -0.01086;0.51552;-4.78240;,
 3.87140;0.51552;-3.98650;,
 3.79494;-0.60277;-4.03125;,
 2.00443;-0.60277;-4.52589;,
 -0.21699;-0.60278;4.48398;,
 0.41838;-0.60278;4.44778;,
 -0.01087;0.51552;4.39657;,
 1.81272;0.51552;-4.47554;,
 2.00443;-0.60277;-4.52589;,
 0.41839;-0.60277;-4.83626;,
 2.00443;-0.60278;4.27801;,
 3.81818;-0.60278;3.86745;,
 3.89812;0.51552;3.82281;,
 4.72529;-0.60277;-2.02171;,
 3.79494;-0.60277;-4.03125;,
 3.87140;0.51552;-3.98650;,
 0.41838;-0.60278;4.44778;,
 2.00443;-0.60278;4.27801;,
 1.81271;0.51552;4.22872;,
 4.99637;-0.60278;-0.00554;,
 4.72529;-0.60277;-2.02171;,
 4.94108;0.51552;-1.99970;,
 4.94108;0.51552;1.98700;,
 4.72529;-0.60278;2.01063;,
 4.99637;-0.60278;-0.00554;,
 3.89812;0.51552;3.82281;,
 3.81818;-0.60278;3.86745;,
 4.72529;-0.60278;2.01063;,
 5.28322;1.67450;2.08249;,
 4.94108;0.51552;1.98700;,
 5.25277;0.51552;-0.00635;,
 4.06792;1.67450;4.01301;,
 3.89812;0.51552;3.82281;,
 4.94108;0.51552;1.98700;,
 -0.21698;1.67450;-4.72649;,
 -0.21698;0.51552;-4.63183;,
 -0.42310;0.51552;-4.78240;,
 3.87140;0.51552;-3.98650;,
 1.81272;0.51552;-4.47554;,
 1.63794;1.67450;-4.71348;,
 1.63794;1.67450;-4.71348;,
 1.81272;0.51552;-4.47554;,
 -0.01086;0.51552;-4.78240;,
 1.81271;0.51552;4.22872;,
 3.89812;0.51552;3.82281;,
 4.06792;1.67450;4.01301;,
 4.94108;0.51552;-1.99970;,
 3.87140;0.51552;-3.98650;,
 4.03678;1.67450;-3.62380;,
 -0.01087;0.51552;4.39657;,
 1.81271;0.51552;4.22872;,
 1.63794;1.67450;4.43986;,
 5.25277;0.51552;-0.00635;,
 4.94108;0.51552;-1.99970;,
 5.28322;1.67450;-2.10990;,
 -0.01086;0.51552;-4.78240;,
 -0.21698;1.67450;-4.72649;,
 4.03678;1.67450;-3.62380;,
 1.63794;1.67450;4.43986;,
 5.64640;1.67450;-0.01370;,
 -0.21699;0.51552;4.43236;,
 -0.21699;1.67450;4.61637;,
 -0.21698;0.51552;-4.63183;,
 0.48001;-1.88503;4.44114;,
 2.22053;-1.88503;-4.51942;,
 2.22053;-1.88503;4.27161;,
 -5.64037;-1.88503;2.00755;,
 -5.15926;-0.60278;2.01063;,
 -5.43034;-0.60278;-0.00554;,
 -4.64491;-1.88503;3.86165;,
 -4.25216;-0.60278;3.86744;,
 -5.15926;-0.60278;2.01063;,
 -0.21698;-1.88503;-4.82934;,
 -0.91398;-1.88503;-4.82934;,
 -0.85235;-0.60277;-4.83626;,
 -2.65450;-1.88503;-4.51942;,
 -4.61940;-1.88503;-4.02551;,
 -4.22891;-0.60277;-4.03125;,
 -0.21699;-0.60278;4.48398;,
 -0.85236;-0.60278;4.44778;,
 -0.91398;-1.88503;-4.82934;,
 -2.65450;-1.88503;-4.51942;,
 -2.43840;-0.60277;-4.52589;,
 -2.65450;-1.88503;4.27161;,
 -2.43840;-0.60278;4.27800;,
 -4.25216;-0.60278;3.86744;,
 -4.61940;-1.88503;-4.02551;,
 -5.64037;-1.88503;-2.01890;,
 -5.15926;-0.60277;-2.02171;,
 -0.91399;-1.88503;4.44114;,
 -0.85236;-0.60278;4.44778;,
 -2.43840;-0.60278;4.27800;,
 -5.64037;-1.88503;-2.01890;,
 -5.93785;-1.88503;-0.00568;,
 -5.43034;-0.60278;-0.00554;,
 -0.85235;-0.60277;-4.83626;,
 -0.42310;0.51552;-4.78240;,
 -0.21698;-0.60277;-4.83626;,
 -2.43840;-0.60277;-4.52589;,
 -4.22891;-0.60277;-4.03125;,
 -4.30536;0.51552;-3.98650;,
 -0.21699;0.51552;4.43236;,
 -0.42311;0.51552;4.39657;,
 -0.85235;-0.60277;-4.83626;,
 -2.43840;-0.60277;-4.52589;,
 -2.24668;0.51552;-4.47554;,
 -2.43840;-0.60278;4.27800;,
 -2.24669;0.51552;4.22872;,
 -4.33209;0.51552;3.82280;,
 -5.15926;-0.60277;-2.02171;,
 -5.37505;0.51552;-1.99970;,
 -4.30536;0.51552;-3.98650;,
 -0.85236;-0.60278;4.44778;,
 -0.42311;0.51552;4.39657;,
 -2.24669;0.51552;4.22872;,
 -5.43034;-0.60278;-0.00554;,
 -5.68673;0.51552;-0.00635;,
 -5.37505;0.51552;-1.99970;,
 -5.43034;-0.60278;-0.00554;,
 -5.15926;-0.60278;2.01063;,
 -5.37505;0.51552;1.98700;,
 -5.15926;-0.60278;2.01063;,
 -4.25216;-0.60278;3.86744;,
 -4.33209;0.51552;3.82280;,
 -5.68673;0.51552;-0.00635;,
 -5.37505;0.51552;1.98700;,
 -5.71719;1.67450;2.08249;,
 -5.37505;0.51552;1.98700;,
 -4.33209;0.51552;3.82280;,
 -4.50189;1.67450;4.01301;,
 -4.30536;0.51552;-3.98650;,
 -4.47075;1.67450;-3.62380;,
 -2.07191;1.67450;-4.71348;,
 -2.24668;0.51552;-4.47554;,
 -2.07191;1.67450;-4.71348;,
 -2.24669;0.51552;4.22872;,
 -2.07191;1.67450;4.43986;,
 -4.50189;1.67450;4.01301;,
 -5.37505;0.51552;-1.99970;,
 -5.71719;1.67450;-2.10990;,
 -4.47075;1.67450;-3.62380;,
 -0.42311;0.51552;4.39657;,
 -0.21699;1.67450;4.61637;,
 -2.07191;1.67450;4.43986;,
 -5.68673;0.51552;-0.00635;,
 -6.08037;1.67450;-0.01371;,
 -5.71719;1.67450;-2.10990;,
 -0.21699;1.67450;4.61637;,
 -0.42311;0.51552;4.39657;,
 -0.42310;0.51552;-4.78240;,
 -2.65450;-1.88503;4.27161;,
 -5.93785;-1.88503;-0.00568;,
 -2.65450;-1.88503;-4.51942;,
 4.72529;-0.60278;2.01063;,
 3.81818;-0.60278;3.86745;,
 2.00443;-0.60277;-4.52589;,
 -0.21699;-0.60278;4.48398;,
 0.41839;-0.60277;-4.83626;,
 2.00443;-0.60278;4.27801;,
 3.79494;-0.60277;-4.03125;,
 0.41838;-0.60278;4.44778;,
 4.72529;-0.60277;-2.02171;,
 1.81272;0.51552;-4.47554;,
 -0.21699;0.51552;4.43236;,
 -0.01086;0.51552;-4.78240;,
 1.81271;0.51552;4.22872;,
 4.94108;0.51552;-1.99970;,
 -0.01087;0.51552;4.39657;,
 5.25277;0.51552;-0.00635;,
 5.25277;0.51552;-0.00635;,
 4.94108;0.51552;1.98700;,
 5.64640;1.67450;-0.01370;,
 5.28322;1.67450;2.08249;,
 4.03678;1.67450;-3.62380;,
 -0.21698;1.67450;-4.72649;,
 1.63794;1.67450;4.43986;,
 5.28322;1.67450;-2.10990;,
 5.64640;1.67450;-0.01370;,
 5.28322;1.67450;2.08249;,
 4.06792;1.67450;4.01301;,
 -0.21698;1.67450;-4.72649;,
 -0.21699;1.67450;4.61637;,
 -2.07191;1.67450;4.43986;,
 1.63794;1.67450;-4.71348;,
 -5.71719;1.67450;2.08249;,
 -4.50189;1.67450;4.01301;,
 -6.08037;1.67450;-0.01371;,
 -4.47075;1.67450;-3.62380;,
 -5.71719;1.67450;-2.10990;,
 -2.07191;1.67450;-4.71348;,
 5.28322;1.67450;-2.10990;,
 4.21094;-1.88503;3.86165;,
 5.20640;-1.88503;2.00755;,
 5.50388;-1.88503;-0.00567;,
 5.20640;-1.88503;-2.01890;,
 4.18543;-1.88503;-4.02551;,
 0.48002;-1.88503;-4.82934;,
 -0.21698;-1.88503;-4.82934;,
 -0.21699;-1.88503;4.47729;,
 -5.93785;-1.88503;-0.00568;,
 -5.64037;-1.88503;2.00755;,
 -2.43840;-0.60277;-4.52589;,
 -0.91399;-1.88503;4.44114;,
 -0.85235;-0.60277;-4.83626;,
 -4.64491;-1.88503;3.86165;,
 -4.22891;-0.60277;-4.03125;,
 -2.65450;-1.88503;4.27161;,
 -5.15926;-0.60277;-2.02171;,
 -2.24668;0.51552;-4.47554;,
 -0.85236;-0.60278;4.44778;,
 -4.25216;-0.60278;3.86744;,
 -4.22891;-0.60277;-4.03125;,
 -2.43840;-0.60278;4.27800;,
 -5.15926;-0.60277;-2.02171;,
 -5.68673;0.51552;-0.00635;,
 -5.37505;0.51552;1.98700;,
 -6.08037;1.67450;-0.01371;,
 -5.71719;1.67450;2.08249;,
 -2.24668;0.51552;-4.47554;,
 -4.33209;0.51552;3.82280;,
 -4.30536;0.51552;-3.98650;,
 -2.24669;0.51552;4.22872;,
 -5.37505;0.51552;-1.99970;,
 -5.64037;-1.88503;2.00755;,
 -4.64491;-1.88503;3.86165;,
 -0.91399;-1.88503;4.44114;,
 -0.91398;-1.88503;-4.82934;,
 -0.21698;-1.88503;-4.82934;,
 -0.21699;-1.88503;4.47729;,
 -5.64037;-1.88503;-2.01890;,
 -4.61940;-1.88503;-4.02551;;
 
 148;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,8;,
 3;9,10,11;,
 3;12,13,14;,
 3;15,16,17;,
 3;18,19,20;,
 3;21,22,23;,
 3;24,25,26;,
 3;27,28,29;,
 3;30,31,32;,
 3;33,34,35;,
 3;36,37,38;,
 3;39,40,41;,
 3;42,43,44;,
 3;45,46,47;,
 3;48,49,50;,
 3;51,52,53;,
 3;54,55,56;,
 3;57,58,59;,
 3;60,61,62;,
 3;63,64,65;,
 3;66,67,68;,
 3;69,70,71;,
 3;72,73,74;,
 3;75,76,77;,
 3;78,79,80;,
 3;81,82,83;,
 3;84,85,86;,
 3;87,67,88;,
 3;89,90,91;,
 3;92,38,93;,
 3;87,31,94;,
 3;95,96,97;,
 3;98,99,100;,
 3;101,102,103;,
 3;104,105,106;,
 3;107,108,109;,
 3;12,110,111;,
 3;112,113,114;,
 3;115,116,117;,
 3;118,119,120;,
 3;121,122,123;,
 3;124,125,126;,
 3;127,128,129;,
 3;130,131,132;,
 3;36,133,134;,
 3;135,136,137;,
 3;138,139,140;,
 3;141,142,143;,
 3;144,145,146;,
 3;147,148,149;,
 3;150,151,152;,
 3;153,154,155;,
 3;156,157,158;,
 3;159,160,161;,
 3;162,163,164;,
 3;68,165,166;,
 3;167,168,169;,
 3;170,171,172;,
 3;173,174,175;,
 3;176,177,178;,
 3;92,179,180;,
 3;181,67,129;,
 3;182,183,184;,
 3;0,2,185;,
 3;3,5,186;,
 3;6,8,31;,
 3;9,11,187;,
 3;12,14,188;,
 3;15,17,189;,
 3;18,20,190;,
 3;21,23,191;,
 3;24,26,192;,
 3;27,29,193;,
 3;33,35,194;,
 3;36,38,195;,
 3;39,41,196;,
 3;42,44,197;,
 3;45,47,198;,
 3;48,50,199;,
 3;51,53,200;,
 3;54,56,201;,
 3;57,59,202;,
 3;60,62,203;,
 3;63,65,204;,
 3;69,71,205;,
 3;72,74,206;,
 3;75,77,207;,
 3;78,80,208;,
 3;81,83,93;,
 3;84,86,209;,
 3;210,90,211;,
 3;90,212,213;,
 3;213,212,214;,
 3;90,215,212;,
 3;214,216,217;,
 3;216,214,218;,
 3;218,219,220;,
 3;214,219,218;,
 3;219,214,221;,
 3;221,214,212;,
 3;215,90,89;,
 3;89,91,222;,
 3;90,210,91;,
 3;223,97,224;,
 3;224,97,225;,
 3;225,96,226;,
 3;226,96,227;,
 3;96,95,228;,
 3;225,97,96;,
 3;228,95,229;,
 3;229,95,230;,
 3;98,100,231;,
 3;101,103,232;,
 3;104,106,31;,
 3;107,109,233;,
 3;12,111,234;,
 3;112,114,235;,
 3;115,117,236;,
 3;118,120,237;,
 3;121,123,238;,
 3;124,126,239;,
 3;130,132,240;,
 3;36,134,241;,
 3;135,137,128;,
 3;138,140,242;,
 3;141,143,243;,
 3;144,146,244;,
 3;147,149,245;,
 3;150,152,246;,
 3;153,155,247;,
 3;156,158,248;,
 3;159,161,249;,
 3;162,164,250;,
 3;68,166,206;,
 3;167,169,251;,
 3;170,172,252;,
 3;173,175,253;,
 3;176,178,254;,
 3;183,182,255;,
 3;255,182,256;,
 3;182,184,257;,
 3;258,259,257;,
 3;257,259,260;,
 3;258,257,184;,
 3;184,261,262;,
 3;261,184,183;;
 
 MeshMaterialList {
  1;
  148;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.640000;0.640000;0.640000;1.000000;;
   0.500000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\character\\enemy_1.png";
   }
  }
 }
 MeshNormals {
  81;
  0.697688;0.232106;0.677760;,
  0.715646;0.237130;-0.656978;,
  0.173293;0.033710;0.984293;,
  0.085080;0.002886;0.996370;,
  0.122910;0.007732;-0.992388;,
  0.228390;0.044803;-0.972538;,
  0.908522;0.346869;-0.232960;,
  0.929818;0.368019;0.000001;,
  0.906990;0.346241;0.239761;,
  0.929730;0.093181;0.356257;,
  0.534719;0.073231;0.841851;,
  0.220480;0.041524;-0.974507;,
  0.564871;0.077219;-0.821558;,
  0.157212;0.034158;0.986974;,
  0.078317;0.033690;0.996359;,
  0.091891;0.038308;-0.995032;,
  0.934808;0.093525;-0.342617;,
  0.996738;0.080708;0.000002;,
  0.918053;-0.227356;0.324789;,
  0.592801;-0.091139;0.800176;,
  0.197327;-0.013489;-0.980245;,
  0.592648;-0.003110;-0.805456;,
  0.140713;-0.047794;0.988896;,
  0.112812;-0.049697;0.992373;,
  -0.148979;0.020218;-0.988634;,
  0.921007;-0.190625;-0.339716;,
  0.959895;-0.280361;0.000033;,
  0.205515;-0.017328;-0.978501;,
  0.716316;-0.012019;-0.697672;,
  -0.697688;0.232107;0.677759;,
  -0.715646;0.237130;-0.656978;,
  -0.173293;0.033717;0.984293;,
  -0.085083;0.002888;0.996370;,
  -0.122910;0.007732;-0.992388;,
  -0.228390;0.044802;-0.972538;,
  -0.908521;0.346869;-0.232960;,
  -0.929818;0.368019;0.000000;,
  -0.906991;0.346241;0.239761;,
  0.000000;-0.003951;0.999992;,
  0.000000;-0.005397;-0.999986;,
  -0.929730;0.093182;0.356257;,
  -0.534719;0.073235;0.841850;,
  -0.220479;0.041525;-0.974507;,
  -0.564871;0.077222;-0.821558;,
  -0.157214;0.034157;0.986974;,
  -0.078319;0.033689;0.996359;,
  0.000000;0.036107;0.999348;,
  0.000000;0.067311;-0.997732;,
  -0.091891;0.038308;-0.995032;,
  -0.934808;0.093526;-0.342617;,
  -0.996738;0.080710;0.000002;,
  -0.918054;-0.227355;0.324788;,
  -0.592802;-0.091141;0.800175;,
  -0.197327;-0.013489;-0.980245;,
  -0.592647;-0.003110;-0.805456;,
  -0.140716;-0.047798;0.988895;,
  -0.112812;-0.049697;0.992373;,
  0.000000;-0.055652;0.998450;,
  0.589394;0.040339;-0.806838;,
  0.148979;0.020217;-0.988634;,
  -0.921008;-0.190625;-0.339716;,
  -0.959895;-0.280360;0.000033;,
  -0.205515;-0.017330;-0.978501;,
  -0.716315;-0.012023;-0.697673;,
  0.912045;-0.301359;0.278132;,
  0.553025;-0.225169;0.802161;,
  0.118343;-0.162224;0.979632;,
  -0.000000;-0.163543;0.986536;,
  0.898672;-0.304609;-0.315597;,
  0.205474;0.011535;-0.978595;,
  0.000000;1.000000;0.000000;,
  -0.118343;-0.162224;0.979632;,
  -0.912045;-0.301361;0.278130;,
  -0.553026;-0.225172;0.802160;,
  -0.898672;-0.304612;-0.315596;,
  -0.946876;-0.321600;0.000053;,
  -0.589394;0.040339;-0.806838;,
  -0.588595;-0.065810;-0.805745;,
  0.000000;-1.000000;-0.000000;,
  0.946878;-0.321593;0.000054;,
  0.000000;1.000000;0.000000;;
  148;
  3;8,7,17;,
  3;0,8,9;,
  3;15,4,39;,
  3;12,1,5;,
  3;38,3,14;,
  3;11,5,4;,
  3;2,0,10;,
  3;16,6,1;,
  3;3,2,13;,
  3;17,7,6;,
  3;15,47,24;,
  3;21,12,11;,
  3;46,14,23;,
  3;20,11,15;,
  3;13,10,19;,
  3;16,12,21;,
  3;14,13,22;,
  3;17,16,25;,
  3;18,9,17;,
  3;19,10,9;,
  3;64,18,26;,
  3;65,19,18;,
  3;69,58,59;,
  3;21,20,27;,
  3;27,20,24;,
  3;22,19,65;,
  3;25,21,28;,
  3;23,22,66;,
  3;26,25,68;,
  3;24,76,77;,
  3;70,70,70;,
  3;57,23,67;,
  3;24,47,76;,
  3;78,78,78;,
  3;37,40,50;,
  3;29,41,40;,
  3;39,33,48;,
  3;34,30,43;,
  3;38,46,45;,
  3;33,34,42;,
  3;31,44,41;,
  3;30,35,49;,
  3;32,45,44;,
  3;35,36,50;,
  3;48,59,47;,
  3;42,43,54;,
  3;46,57,56;,
  3;48,42,53;,
  3;44,55,52;,
  3;49,60,54;,
  3;45,56,55;,
  3;50,61,60;,
  3;50,40,51;,
  3;40,41,52;,
  3;61,51,72;,
  3;51,52,73;,
  3;54,63,62;,
  3;59,53,62;,
  3;55,71,73;,
  3;60,74,63;,
  3;56,67,71;,
  3;61,75,74;,
  3;57,67,56;,
  3;59,58,47;,
  3;78,78,78;,
  3;8,17,9;,
  3;0,9,10;,
  3;15,39,47;,
  3;12,5,11;,
  3;38,14,46;,
  3;11,4,15;,
  3;2,10,13;,
  3;16,1,12;,
  3;3,13,14;,
  3;17,6,16;,
  3;21,11,20;,
  3;46,23,57;,
  3;20,15,24;,
  3;13,19,22;,
  3;16,21,25;,
  3;14,22,23;,
  3;17,25,26;,
  3;18,17,26;,
  3;19,9,18;,
  3;64,26,79;,
  3;65,18,64;,
  3;21,27,28;,
  3;27,24,69;,
  3;22,65,66;,
  3;25,28,68;,
  3;23,66,67;,
  3;26,68,79;,
  3;70,70,80;,
  3;70,70,70;,
  3;70,70,70;,
  3;70,70,70;,
  3;70,70,70;,
  3;70,70,70;,
  3;70,70,70;,
  3;70,70,70;,
  3;70,70,70;,
  3;70,70,70;,
  3;70,70,70;,
  3;70,70,70;,
  3;70,70,70;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;37,50,36;,
  3;29,40,37;,
  3;39,48,47;,
  3;34,43,42;,
  3;38,45,32;,
  3;33,42,48;,
  3;31,41,29;,
  3;30,49,43;,
  3;32,44,31;,
  3;35,50,49;,
  3;42,54,53;,
  3;46,56,45;,
  3;48,53,59;,
  3;44,52,41;,
  3;49,54,43;,
  3;45,55,44;,
  3;50,60,49;,
  3;50,51,61;,
  3;40,52,51;,
  3;61,72,75;,
  3;51,73,72;,
  3;54,62,53;,
  3;59,62,69;,
  3;55,73,52;,
  3;60,63,54;,
  3;56,71,55;,
  3;61,74,60;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;,
  3;78,78,78;;
 }
 MeshTextureCoords {
  263;
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.196510;0.560510;,
  0.259490;0.592000;,
  0.215950;0.621090;,
  0.250270;0.614270;,
  0.259490;0.560510;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.259170;0.587520;,
  0.204200;0.571380;,
  0.256640;0.605080;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.259490;0.560510;,
  0.259490;0.623490;,
  0.196510;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.259490;0.560510;,
  0.199360;0.605080;,
  0.245020;0.618490;,
  0.251800;0.571380;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.623490;,
  0.196510;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.240050;0.621090;,
  0.228000;0.623490;,
  0.250270;0.569730;,
  0.205730;0.614270;,
  0.198910;0.604050;,
  0.257090;0.579950;,
  0.198910;0.579950;,
  0.196510;0.592000;,
  0.205730;0.569730;,
  0.228000;0.560510;,
  0.215950;0.562910;,
  0.240050;0.562910;,
  0.257090;0.604050;,
  0.245020;0.618490;,
  0.228000;0.623490;,
  0.210980;0.618490;,
  0.199360;0.605080;,
  0.196830;0.587520;,
  0.219130;0.561790;,
  0.236870;0.561790;,
  0.251800;0.571380;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.623490;,
  0.196510;0.623490;,
  0.196510;0.623490;,
  0.196510;0.623490;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.196510;0.560510;,
  0.228000;0.623490;,
  0.210980;0.618490;,
  0.196830;0.587520;,
  0.236870;0.561790;,
  0.219130;0.561790;,
  0.204200;0.571380;,
  0.256640;0.605080;,
  0.259170;0.587520;;
 }
}

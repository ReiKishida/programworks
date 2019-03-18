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
 18;
 0.88178;-5.38588;8.50898;,
 9.32127;-5.38589;7.10337;,
 7.72687;-0.62390;5.77474;,
 0.88178;-0.28050;6.91480;,
 9.32127;-5.38589;-6.96209;,
 7.72687;-0.62390;-5.63346;,
 0.88178;-5.38588;-8.36770;,
 -7.55771;-5.38588;-6.96209;,
 -5.96331;-0.62389;-5.63346;,
 0.88178;-0.28050;-6.77352;,
 -7.55771;-5.38588;7.10337;,
 -5.96331;-0.62389;5.77474;,
 0.88178;-7.44740;6.91480;,
 -5.96331;-7.10400;5.77474;,
 -5.96331;-7.10400;-5.63346;,
 0.88178;-7.44740;-6.77352;,
 7.72687;-7.10400;5.77474;,
 7.72687;-7.10400;-5.63346;;
 
 16;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;6,7,8,9;,
 4;7,10,11,8;,
 4;12,13,14,15;,
 4;9,8,11,3;,
 4;14,13,10,7;,
 4;15,14,7,6;,
 4;16,17,4,1;,
 4;12,16,1,0;,
 4;13,12,0,10;,
 4;17,15,6,4;,
 4;5,9,3,2;,
 4;16,12,15,17;,
 4;4,6,9,5;,
 4;10,0,3,11;;
 
 MeshMaterialList {
  3;
  16;
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
   0.106400;0.106400;0.106400;1.000000;;
   58.000000;
   0.360000;0.360000;0.360000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.062400;0.078400;0.031200;1.000000;;
   38.000000;
   0.550000;0.550000;0.550000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.109600;0.200800;0.081600;1.000000;;
   49.000000;
   0.380000;0.380000;0.380000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  25;
  -0.354280;-0.889193;0.289520;,
  -0.948260;0.317495;0.000000;,
  0.354279;-0.889193;0.289519;,
  0.163695;0.303635;0.938621;,
  -0.354280;-0.889193;-0.289520;,
  -0.163695;0.303635;-0.938621;,
  0.354279;-0.889193;-0.289519;,
  0.948260;0.317495;0.000000;,
  0.173721;-0.191961;0.965905;,
  -0.173721;-0.191960;-0.965905;,
  -0.000000;-0.908239;0.418453;,
  0.000000;0.307787;0.951455;,
  -0.000000;-0.908239;-0.418453;,
  0.000000;0.307787;-0.951455;,
  -0.000000;-0.194924;-0.980818;,
  -0.000000;-0.194924;0.980818;,
  0.000001;1.000000;0.000000;,
  -0.050103;0.998744;0.000000;,
  -0.498164;-0.752696;0.430442;,
  -0.733006;-0.680223;0.000000;,
  0.498163;-0.752698;-0.430442;,
  0.733004;-0.680225;-0.000000;,
  0.050104;0.998744;0.000000;,
  0.163695;0.303635;-0.938621;,
  -0.163695;0.303635;0.938621;;
  16;
  4;15,8,3,11;,
  4;7,7,7,7;,
  4;14,9,5,13;,
  4;1,1,1,1;,
  4;10,0,4,12;,
  4;16,17,17,16;,
  4;4,0,18,19;,
  4;12,4,9,14;,
  4;2,6,20,21;,
  4;10,2,8,15;,
  4;0,10,15,18;,
  4;6,12,14,20;,
  4;22,16,16,22;,
  4;2,10,12,6;,
  4;23,14,13,23;,
  4;24,15,11,24;;
 }
 MeshTextureCoords {
  18;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}

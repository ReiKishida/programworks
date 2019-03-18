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
 24;
 91.96685;0.86849;-168.06060;,
 91.96686;0.86849;182.83457;,
 -60.39330;0.86849;182.83457;,
 -60.39331;0.86849;-168.06060;,
 91.96685;184.24663;-168.06077;,
 -60.39337;184.24663;-168.06055;,
 -60.39330;184.24663;182.83472;,
 91.96686;184.24663;182.83457;,
 91.96685;0.86849;-168.06060;,
 -60.39331;0.86849;-168.06060;,
 -60.39337;184.24663;-168.06055;,
 91.96685;184.24663;-168.06077;,
 -60.39331;0.86849;-168.06060;,
 -60.39330;0.86849;182.83457;,
 -60.39330;184.24663;182.83472;,
 -60.39337;184.24663;-168.06055;,
 -60.39330;0.86849;182.83457;,
 91.96686;0.86849;182.83457;,
 91.96686;184.24663;182.83457;,
 -60.39330;184.24663;182.83472;,
 91.96685;184.24663;-168.06077;,
 91.96686;184.24663;182.83457;,
 91.96686;0.86849;182.83457;,
 91.96685;0.86849;-168.06060;;
 
 6;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;;
 
 MeshMaterialList {
  1;
  6;
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.000000;0.131765;1.000000;;
   43.000000;
   0.600000;0.600000;0.600000;;
   0.180000;0.000000;0.029647;;
   TextureFilename {
    "Visual Studio 2015\\Projects\\制作応用(3D)\\3Dゲーム制作_303\\data\\TEXTURE\\contena.png";
   }
  }
 }
 MeshNormals {
  6;
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -0.000001;-0.000000;-1.000000;,
  -1.000000;-0.000000;0.000000;,
  0.000000;-0.000000;1.000000;,
  1.000000;-0.000000;-0.000000;;
  6;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;;
 }
 MeshTextureCoords {
  24;
  0.666470;0.666870;,
  0.666470;0.999800;,
  0.333530;0.999800;,
  0.333530;0.666870;,
  0.999900;1.002930;,
  0.753430;1.002930;,
  0.753430;0.007070;,
  0.999900;0.007070;,
  0.253340;0.511080;,
  -0.001900;0.511080;,
  -0.001900;0.001750;,
  0.253340;0.001750;,
  0.754800;0.509800;,
  0.251870;0.509800;,
  0.251870;0.003530;,
  0.754800;0.003530;,
  0.001620;0.503740;,
  0.248930;0.503740;,
  0.248930;1.000460;,
  0.001620;1.000460;,
  0.249240;0.512570;,
  0.752430;0.512570;,
  0.752430;1.004100;,
  0.249240;1.004100;;
 }
}

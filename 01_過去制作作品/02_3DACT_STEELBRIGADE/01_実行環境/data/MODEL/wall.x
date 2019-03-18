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
 92;
 -81.47410;1.37031;1476.60666;,
 -81.47410;1.37031;2067.24984;,
 -32.12614;413.63736;2067.24984;,
 -32.12614;413.63736;1476.60666;,
 -81.47410;1.37031;2067.24984;,
 81.47410;1.37031;2067.24984;,
 32.12614;413.63736;2067.24984;,
 -32.12614;413.63736;2067.24984;,
 81.47410;1.37031;-1476.60666;,
 81.47410;1.37031;-2067.24984;,
 32.12614;413.63746;-2067.24984;,
 32.12614;413.63736;-1476.60666;,
 81.47410;1.37031;-2067.24984;,
 -81.47410;1.37031;-2067.24984;,
 -32.12614;413.63746;-2067.24984;,
 32.12614;413.63746;-2067.24984;,
 -81.47410;1.37031;-1476.60666;,
 81.47410;1.37031;-2067.24984;,
 81.47410;1.37031;-1476.60666;,
 32.12614;413.63736;-1476.60666;,
 32.12614;413.63746;-2067.24984;,
 -32.12614;413.63736;-1476.60666;,
 32.12614;413.63736;2067.24984;,
 32.12614;413.63736;1476.60666;,
 -32.12614;413.63736;1476.60666;,
 32.12614;413.63736;1476.60666;,
 32.12614;413.63736;885.96476;,
 -32.12614;413.63736;885.96476;,
 32.12614;413.63736;885.96476;,
 32.12614;413.63736;295.32173;,
 -32.12614;413.63736;295.32173;,
 -32.12614;413.63736;885.96476;,
 32.12614;413.63736;295.32173;,
 32.12614;413.63736;-295.32166;,
 -32.12614;413.63736;-295.32166;,
 -32.12614;413.63736;295.32173;,
 32.12614;413.63736;-295.32166;,
 32.12614;413.63736;-885.96476;,
 -32.12614;413.63736;-885.96476;,
 -32.12614;413.63736;-295.32166;,
 32.12614;413.63736;-885.96476;,
 32.12614;413.63736;-1476.60666;,
 -32.12614;413.63736;-1476.60666;,
 -32.12614;413.63736;-885.96476;,
 -81.47410;1.37031;1476.60666;,
 81.47410;1.37031;1476.60666;,
 81.47410;1.37031;2067.24984;,
 -81.47410;1.37031;885.96476;,
 81.47410;1.37031;885.96476;,
 81.47410;1.37031;1476.60666;,
 -81.47410;1.37031;885.96476;,
 -81.47410;1.37031;295.32171;,
 81.47410;1.37031;295.32171;,
 81.47410;1.37031;885.96476;,
 -81.47410;1.37031;295.32171;,
 -81.47410;1.37031;-295.32171;,
 81.47410;1.37031;-295.32171;,
 81.47410;1.37031;295.32171;,
 -81.47410;1.37031;-295.32171;,
 -81.47410;1.37031;-885.96476;,
 81.47410;1.37031;-885.96476;,
 81.47410;1.37031;-295.32171;,
 -81.47410;1.37031;-885.96476;,
 -81.47410;1.37031;-1476.60666;,
 81.47410;1.37031;-1476.60666;,
 81.47410;1.37031;-885.96476;,
 81.47410;1.37031;2067.24984;,
 81.47410;1.37031;1476.60666;,
 32.12614;413.63736;1476.60666;,
 32.12614;413.63736;2067.24984;,
 81.47410;1.37031;1476.60666;,
 81.47410;1.37031;885.96476;,
 32.12614;413.63736;885.96476;,
 32.12614;413.63736;1476.60666;,
 81.47410;1.37031;885.96476;,
 81.47410;1.37031;295.32171;,
 32.12614;413.63736;295.32173;,
 32.12614;413.63736;885.96476;,
 81.47410;1.37031;295.32171;,
 81.47410;1.37031;-295.32171;,
 32.12614;413.63736;-295.32166;,
 32.12614;413.63736;295.32173;,
 81.47410;1.37031;-295.32171;,
 81.47410;1.37031;-885.96476;,
 32.12614;413.63736;-885.96476;,
 32.12614;413.63736;-295.32166;,
 81.47410;1.37031;-885.96476;,
 81.47410;1.37031;-1476.60666;,
 32.12614;413.63736;-1476.60666;,
 32.12614;413.63736;-885.96476;,
 -81.47410;1.37031;-2067.24984;,
 -32.12614;413.63746;-2067.24984;;
 
 30;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,13,17,18;,
 4;19,20,14,21;,
 4;22,23,24,7;,
 4;25,26,27,3;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;4,44,45,46;,
 4;0,47,48,49;,
 4;50,51,52,53;,
 4;54,55,56,57;,
 4;58,59,60,61;,
 4;62,63,64,65;,
 4;66,67,68,69;,
 4;70,71,72,73;,
 4;74,75,76,77;,
 4;78,79,80,81;,
 4;82,83,84,85;,
 4;86,87,88,89;,
 4;90,63,42,91;,
 4;16,59,38,21;,
 4;62,55,34,43;,
 4;58,51,30,39;,
 4;54,47,27,35;,
 4;50,44,24,31;;
 
 MeshMaterialList {
  1;
  30;
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
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game111\\Documents\\Visual Studio 2015\\Projects\\制作応用(3D)\\3Dゲーム制作_301\\data\\TEXTURE\\wall02.jpg";
   }
  }
 }
 MeshNormals {
  16;
  0.000000;0.000000;-1.000000;,
  -0.992912;0.118851;0.000000;,
  0.992912;0.118851;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.992912;0.118851;0.000000;,
  0.000000;1.000000;0.000000;,
  0.992912;0.118851;0.000000;,
  0.992912;0.118851;0.000000;,
  0.992912;0.118851;0.000000;,
  -0.992912;0.118851;0.000000;,
  -0.992912;0.118851;0.000000;,
  -0.992912;0.118851;0.000000;,
  -0.992912;0.118851;0.000000;;
  30;
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;7,2,2,7;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;5,8,8,5;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;6,5,5,6;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;9,9,9,9;,
  4;9,10,10,9;,
  4;10,9,9,10;,
  4;9,11,11,9;,
  4;11,11,11,11;,
  4;11,7,7,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,14,14,14;,
  4;14,1,1,14;,
  4;1,15,15,1;,
  4;15,1,1,15;;
 }
 MeshTextureCoords {
  92;
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  -0.285090;1.285090;,
  -0.285090;-0.285090;,
  1.285090;-0.285090;,
  1.285090;1.285090;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;;
 }
}

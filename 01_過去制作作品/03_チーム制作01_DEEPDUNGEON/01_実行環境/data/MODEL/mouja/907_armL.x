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
 15;
 6.38689;-1.34720;-0.11947;,
 -0.22699;-1.72023;0.45441;,
 0.16735;0.69062;-1.15791;,
 6.51367;0.63294;-1.10832;,
 6.51367;0.63294;-1.10832;,
 0.16735;0.69062;-1.15791;,
 0.12612;0.69801;1.64135;,
 6.10759;0.07649;1.43411;,
 12.64221;0.86413;-1.36091;,
 11.96916;-1.43885;-0.20178;,
 12.46792;0.98573;1.04233;,
 12.64221;0.86413;-1.36091;,
 0.12612;0.69801;1.64135;,
 12.46792;0.98573;1.04233;,
 6.10759;0.07649;1.43411;;
 
 8;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;0,3,8,9;,
 4;4,7,10,11;,
 3;1,12,2;,
 3;9,8,13;,
 4;12,1,0,14;,
 4;0,9,13,14;;
 
 MeshMaterialList {
  1;
  8;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.409600;0.224000;0.154400;1.000000;;
   0.500000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  11;
  -0.020320;-0.445210;-0.895196;,
  -0.084261;0.993965;0.070233;,
  -0.019774;-0.472769;-0.880964;,
  -0.014344;0.995912;0.089177;,
  -0.019210;-0.499873;-0.865886;,
  0.055648;0.992630;0.107653;,
  -0.988320;0.151656;-0.014956;,
  0.966986;-0.241163;0.082330;,
  0.043558;-0.592275;0.804558;,
  0.052061;-0.603595;0.795589;,
  0.060549;-0.614746;0.786398;;
  8;
  4;2,4,4,2;,
  4;3,5,5,3;,
  4;2,2,0,0;,
  4;3,3,1,1;,
  3;6,6,6;,
  3;7,7,7;,
  4;8,8,9,9;,
  4;9,10,10,9;;
 }
 MeshTextureCoords {
  15;
  0.687180;0.610020;,
  0.677540;0.489170;,
  0.715350;0.485510;,
  0.714360;0.612540;,
  0.772920;0.610400;,
  0.774440;0.491800;,
  0.725900;0.495810;,
  0.729190;0.610520;,
  0.715350;0.779710;,
  0.683490;0.783540;,
  0.730020;0.780480;,
  0.773960;0.781800;,
  0.715350;0.485510;,
  0.715350;0.779710;,
  0.714360;0.612540;;
 }
}

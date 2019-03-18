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
 50;
 1.35182;4.29044;-1.35182;,
 -1.35182;4.29044;-1.35182;,
 -1.35182;5.37191;-1.35182;,
 1.35182;5.37191;-1.35182;,
 0.81109;5.37191;-0.81109;,
 -0.81109;5.37191;-0.81109;,
 -0.81109;5.37191;0.81109;,
 0.81109;5.37191;0.81109;,
 1.35182;5.37191;1.35182;,
 -1.35182;5.37191;1.35182;,
 -1.35182;4.29044;1.35182;,
 1.35182;4.29044;1.35182;,
 0.81109;-0.03539;0.81109;,
 -0.81109;-0.03539;0.81109;,
 -0.81109;-0.03539;-0.81109;,
 0.81109;-0.03539;-0.81109;,
 1.35182;4.29044;1.35182;,
 1.35182;5.37191;1.35182;,
 0.81109;1.04607;-0.81109;,
 -0.81109;1.04607;-0.81109;,
 -0.81109;4.29044;-0.81109;,
 0.81109;4.29044;-0.81109;,
 -0.81109;1.04607;0.81109;,
 -0.81109;4.29044;0.81109;,
 0.81109;4.29044;0.81109;,
 0.81109;1.04607;0.81109;,
 0.81109;1.04607;0.81109;,
 0.81109;4.29044;0.81109;,
 1.35182;-0.03539;-1.35182;,
 -1.35182;-0.03539;-1.35182;,
 -1.35182;1.04607;-1.35182;,
 1.35182;1.04607;-1.35182;,
 -1.35182;-0.03539;1.35182;,
 -1.35182;1.04607;1.35182;,
 1.35182;1.04607;1.35182;,
 1.35182;-0.03539;1.35182;,
 1.35182;-0.03539;1.35182;,
 1.35182;1.04607;1.35182;,
 1.35182;5.37191;-1.35182;,
 1.35182;5.37191;1.35182;,
 1.35182;-0.03539;-1.35182;,
 1.35182;-0.03539;1.35182;,
 0.81109;5.37191;0.81109;,
 -0.81109;5.37191;0.81109;,
 -0.81109;5.37191;-0.81109;,
 0.81109;5.37191;-0.81109;,
 -1.35182;5.37191;-1.35182;,
 1.35182;5.37191;-1.35182;,
 -1.35182;5.37191;1.35182;,
 1.35182;5.37191;1.35182;;
 
 35;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;1,10,9,2;,
 4;16,0,3,17;,
 4;18,19,20,21;,
 4;19,22,23,20;,
 4;24,23,22,25;,
 4;26,18,21,27;,
 4;28,29,30,31;,
 4;29,32,33,30;,
 4;34,33,32,35;,
 4;36,28,31,37;,
 4;21,20,1,0;,
 4;5,4,38,2;,
 4;7,6,9,39;,
 4;23,24,11,10;,
 4;20,23,10,1;,
 4;6,5,2,9;,
 4;27,21,0,16;,
 4;4,7,39,38;,
 4;15,14,29,40;,
 4;19,18,31,30;,
 4;14,13,32,29;,
 4;22,19,30,33;,
 4;25,22,33,34;,
 4;13,12,41,32;,
 4;12,15,40,41;,
 4;18,26,37,31;,
 4;42,43,44,45;,
 4;45,44,46,47;,
 4;44,43,48,46;,
 4;43,42,49,48;,
 4;42,45,47,49;;
 
 MeshMaterialList {
  1;
  35;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/skeleton.png";
   }
  }
 }
 MeshNormals {
  6;
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  1.000000;0.000000;0.000000;;
  35;
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;5,5,5,5;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;;
 }
 MeshTextureCoords {
  50;
  0.925960;0.832850;,
  0.859180;0.832850;,
  0.870340;0.877370;,
  0.925960;0.877370;,
  0.859220;0.966520;,
  0.859220;0.899660;,
  0.792360;0.899660;,
  0.792360;0.966520;,
  0.725610;0.877370;,
  0.781230;0.877370;,
  0.792400;0.832850;,
  0.725610;0.832850;,
  0.791930;0.501510;,
  0.791930;0.569230;,
  0.859650;0.569230;,
  0.859650;0.501510;,
  0.992750;0.832850;,
  0.992750;0.877370;,
  0.925960;0.667800;,
  0.859180;0.667800;,
  0.859180;0.801370;,
  0.925960;0.801370;,
  0.792400;0.667800;,
  0.792400;0.801370;,
  0.725610;0.801370;,
  0.725610;0.667800;,
  0.992750;0.667800;,
  0.992750;0.801370;,
  0.925960;0.591800;,
  0.870700;0.591800;,
  0.859180;0.636320;,
  0.925960;0.636320;,
  0.780880;0.591800;,
  0.792400;0.636320;,
  0.725610;0.636320;,
  0.725610;0.591800;,
  0.992750;0.591800;,
  0.992750;0.636320;,
  0.881510;0.988810;,
  0.770070;0.988810;,
  0.882220;0.478940;,
  0.769360;0.478940;,
  0.791930;0.501510;,
  0.791930;0.569230;,
  0.859650;0.569230;,
  0.859650;0.501510;,
  0.870700;0.591800;,
  0.882220;0.478940;,
  0.780880;0.591800;,
  0.769360;0.478940;;
 }
}

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
 -1.52023;21.76111;53.90415;,
 -1.52023;25.74507;53.90402;,
 -1.52024;25.74177;-44.61987;,
 -1.52024;21.75782;-44.61973;,
 -1.52025;39.49905;53.74601;,
 -1.52025;43.47814;53.94193;,
 -1.52024;48.36508;-44.46171;,
 -1.52024;44.38599;-44.65762;,
 -2.07703;46.12088;52.21785;,
 -2.07703;49.92020;51.02421;,
 -2.07702;20.14635;-42.93354;,
 -2.07702;16.34704;-41.73990;,
 -2.22212;2.56003;53.32959;,
 -2.22212;6.51637;53.79580;,
 -2.22211;18.14538;-44.04529;,
 -2.22211;14.18904;-44.51149;,
 -2.47301;26.20310;52.64388;,
 -2.47301;30.04229;51.58425;,
 -2.47299;3.61119;-43.35957;,
 -2.47299;-0.22800;-42.29994;,
 -2.25102;21.62247;52.81296;,
 -2.25102;25.54601;53.50117;,
 -2.25101;42.71250;-43.52865;,
 -2.25101;38.78897;-44.21686;;
 
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
   0.640000;0.640000;0.640000;1.000000;;
   0.500000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\object\\keepout.png";
   }
  }
 }
 MeshNormals {
  6;
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;-0.000000;,
  -1.000000;-0.000000;-0.000000;,
  -1.000000;0.000000;-0.000000;,
  -1.000000;-0.000000;-0.000000;,
  -1.000000;0.000000;-0.000000;;
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
  0.000040;0.999960;,
  0.000040;0.000040;,
  0.987190;0.000040;,
  0.987190;0.999960;,
  0.000100;0.999900;,
  0.000100;0.000100;,
  0.987060;0.000100;,
  0.987060;0.999900;,
  0.000040;0.999960;,
  0.000040;0.000040;,
  0.987190;0.000040;,
  0.987190;0.999960;,
  0.000040;0.999960;,
  0.000040;0.000040;,
  1.001190;0.000040;,
  1.001190;0.999960;,
  0.000040;0.999960;,
  0.000040;0.000040;,
  0.998860;0.000040;,
  0.998860;0.999960;,
  0.000040;0.999960;,
  0.000040;0.000040;,
  0.998860;0.000040;,
  0.998860;0.999960;;
 }
}

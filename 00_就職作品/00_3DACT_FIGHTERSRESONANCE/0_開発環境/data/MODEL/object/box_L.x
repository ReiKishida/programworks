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
 100;
 -55.79394;111.82103;-52.89711;,
 -55.79394;111.82103;4.06170;,
 -55.79394;168.77990;4.06170;,
 -55.79394;168.77990;-52.89711;,
 -55.79394;111.82103;4.06170;,
 1.16484;111.82103;4.06170;,
 1.16484;168.77990;4.06170;,
 -55.79394;168.77990;4.06170;,
 1.16484;111.82103;4.06170;,
 1.16484;111.82103;-52.89711;,
 1.16484;168.77990;-52.89711;,
 1.16484;168.77990;4.06170;,
 1.16484;111.82103;-52.89711;,
 -55.79394;111.82103;-52.89711;,
 -55.79394;168.77990;-52.89711;,
 1.16484;168.77990;-52.89711;,
 1.16484;111.82103;-52.89711;,
 1.16484;111.82103;4.06170;,
 1.16484;168.77990;4.06170;,
 1.16484;168.77990;-52.89711;,
 -55.55098;-1.22520;-52.72011;,
 -55.55098;-1.22522;59.63361;,
 -55.55098;111.01422;59.63361;,
 -55.55098;111.01422;-52.72011;,
 -55.55098;-1.22522;59.63361;,
 59.81694;-1.22522;59.63361;,
 59.81694;111.01422;59.63361;,
 -55.55098;111.01422;59.63361;,
 59.81694;-1.22522;59.63361;,
 59.81694;-1.22520;-52.72011;,
 59.81694;111.01422;-52.72011;,
 59.81694;111.01422;59.63361;,
 59.81694;-1.22520;-52.72011;,
 -55.55098;-1.22520;-52.72011;,
 -55.55098;111.01422;-52.72011;,
 59.81694;111.01422;-52.72011;,
 59.81694;-1.22520;-52.72011;,
 59.81694;-1.22522;59.63361;,
 59.81694;111.01422;59.63361;,
 59.81694;111.01422;-52.72011;,
 1.46935;111.82103;-52.89711;,
 1.46935;111.82103;4.06170;,
 1.46935;168.77990;4.06170;,
 1.46935;168.77990;-52.89711;,
 1.46935;111.82103;4.06170;,
 58.42814;111.82103;4.06170;,
 58.42814;168.77990;4.06170;,
 1.46935;168.77990;4.06170;,
 58.42814;111.82103;4.06170;,
 58.42814;111.82103;-52.89711;,
 58.42814;168.77990;-52.89711;,
 58.42814;168.77990;4.06170;,
 58.42814;111.82103;-52.89711;,
 1.46935;111.82103;-52.89711;,
 1.46935;168.77990;-52.89711;,
 58.42814;168.77990;-52.89711;,
 58.42814;111.82103;-52.89711;,
 58.42814;111.82103;4.06170;,
 58.42814;168.77990;4.06170;,
 58.42814;168.77990;-52.89711;,
 -55.79394;111.82103;3.84800;,
 -55.79394;111.82103;60.80681;,
 -55.79394;168.77990;60.80681;,
 -55.79394;168.77990;3.84800;,
 -55.79394;111.82103;60.80681;,
 1.16484;111.82103;60.80681;,
 1.16484;168.77990;60.80681;,
 -55.79394;168.77990;60.80681;,
 1.16484;111.82103;60.80681;,
 1.16484;111.82103;3.84800;,
 1.16484;168.77990;3.84800;,
 1.16484;168.77990;60.80681;,
 1.16484;111.82103;3.84800;,
 -55.79394;111.82103;3.84800;,
 -55.79394;168.77990;3.84800;,
 1.16484;168.77990;3.84800;,
 1.16484;111.82103;3.84800;,
 1.16484;111.82103;60.80681;,
 1.16484;168.77990;60.80681;,
 1.16484;168.77990;3.84800;,
 1.62998;111.82103;3.84800;,
 1.62998;111.82103;60.80681;,
 1.62998;168.77990;60.80681;,
 1.62998;168.77990;3.84800;,
 1.62998;111.82103;60.80681;,
 58.58876;111.82103;60.80681;,
 58.58876;168.77990;60.80681;,
 1.62998;168.77990;60.80681;,
 58.58876;111.82103;60.80681;,
 58.58876;111.82103;3.84800;,
 58.58876;168.77990;3.84800;,
 58.58876;168.77990;60.80681;,
 58.58876;111.82103;3.84800;,
 1.62998;111.82103;3.84800;,
 1.62998;168.77990;3.84800;,
 58.58876;168.77990;3.84800;,
 58.58876;111.82103;3.84800;,
 58.58876;111.82103;60.80681;,
 58.58876;168.77990;60.80681;,
 58.58876;168.77990;3.84800;;
 
 30;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;4,13,16,17;,
 4;18,19,14,7;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;24,33,36,37;,
 4;38,39,34,27;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;44,53,56,57;,
 4;58,59,54,47;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;64,73,76,77;,
 4;78,79,74,67;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;84,93,96,97;,
 4;98,99,94,87;;
 
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
   0.500000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\object\\tex.jpg";
   }
  }
 }
 MeshNormals {
  30;
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;;
  30;
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;8,8,8,8;,
  4;7,7,7,7;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;12,12,12,12;,
  4;14,14,14,14;,
  4;13,13,13,13;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;18,18,18,18;,
  4;20,20,20,20;,
  4;19,19,19,19;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;23,23,23,23;,
  4;24,24,24,24;,
  4;26,26,26,26;,
  4;25,25,25,25;,
  4;27,27,27,27;,
  4;28,28,28,28;,
  4;29,29,29,29;;
 }
 MeshTextureCoords {
  100;
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
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
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
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
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
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
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
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
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
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;;
 }
}

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
 54;
 0.06891;0.39780;-21.31884;,
 1.66370;0.32091;-16.91327;,
 0.06891;-0.08166;-12.69375;,
 1.34715;0.19678;-9.80222;,
 0.06891;-0.54803;-3.16319;,
 0.06891;-0.54803;-3.16319;,
 3.25432;0.07835;-3.01747;,
 0.06891;-0.93934;-2.67043;,
 2.90483;-0.26343;-2.54656;,
 2.99789;0.06199;-2.08040;,
 1.94145;0.09427;-3.92944;,
 1.94145;0.09427;-3.92944;,
 1.05199;0.05723;-1.80732;,
 0.06891;-0.71361;0.59293;,
 1.05199;0.05723;-1.80732;,
 0.79773;0.01511;0.60564;,
 1.14434;-0.02683;3.01038;,
 0.06891;-1.10208;2.99162;,
 0.06891;-0.50855;-1.55874;,
 0.06891;-0.50855;-1.55874;,
 0.06891;-0.05510;4.62818;,
 0.06891;-1.45124;3.98992;,
 1.69491;-0.04439;4.01507;,
 0.06891;-1.67975;3.41125;,
 0.06891;-1.10208;2.99162;,
 1.14434;-0.02683;3.01038;,
 1.71454;-0.03437;3.43998;,
 0.06891;0.57596;-12.68227;,
 0.06891;0.70944;-3.14124;,
 0.06891;1.08331;-2.63512;,
 0.06891;0.70944;-3.14124;,
 2.90483;0.40349;-2.53493;,
 0.06891;0.74382;0.61836;,
 0.06891;1.04835;3.02915;,
 0.06891;0.61399;-1.53915;,
 0.06891;0.61399;-1.53915;,
 0.06891;1.36245;4.03904;,
 0.06891;1.04835;3.02915;,
 0.06891;1.61102;3.46869;,
 -1.52589;0.32091;-16.91327;,
 -1.20933;0.19678;-9.80222;,
 -3.11650;0.07835;-3.01747;,
 -2.76702;-0.26343;-2.54656;,
 -2.86008;0.06199;-2.08040;,
 -1.80364;0.09427;-3.92944;,
 -1.80364;0.09427;-3.92944;,
 -0.91418;0.05723;-1.80732;,
 -0.65992;0.01511;0.60564;,
 -0.91418;0.05723;-1.80732;,
 -1.00652;-0.02683;3.01038;,
 -1.55710;-0.04439;4.01507;,
 -1.00652;-0.02683;3.01038;,
 -1.57673;-0.03437;3.43998;,
 -2.76702;0.40349;-2.53493;;
 
 80;
 3;0,1,2;,
 3;3,2,1;,
 3;3,4,2;,
 3;5,6,7;,
 3;7,6,8;,
 3;7,8,9;,
 3;9,8,6;,
 3;10,4,3;,
 3;5,11,6;,
 3;7,9,12;,
 3;13,14,15;,
 3;16,17,13;,
 3;16,13,15;,
 3;7,12,18;,
 3;19,14,13;,
 3;20,21,22;,
 3;23,24,25;,
 3;23,25,26;,
 3;22,21,23;,
 3;22,23,26;,
 3;27,1,0;,
 3;1,27,3;,
 3;27,28,3;,
 3;29,6,30;,
 3;31,6,29;,
 3;9,31,29;,
 3;6,31,9;,
 3;3,28,10;,
 3;6,11,30;,
 3;12,9,29;,
 3;15,14,32;,
 3;32,33,16;,
 3;15,32,16;,
 3;34,12,29;,
 3;32,14,35;,
 3;22,36,20;,
 3;25,37,38;,
 3;26,25,38;,
 3;38,36,22;,
 3;26,38,22;,
 3;2,39,0;,
 3;39,2,40;,
 3;2,4,40;,
 3;7,41,5;,
 3;42,41,7;,
 3;43,42,7;,
 3;41,42,43;,
 3;40,4,44;,
 3;41,45,5;,
 3;46,43,7;,
 3;47,48,13;,
 3;13,17,49;,
 3;47,13,49;,
 3;18,46,7;,
 3;13,48,19;,
 3;50,21,20;,
 3;51,24,23;,
 3;52,51,23;,
 3;23,21,50;,
 3;52,23,50;,
 3;0,39,27;,
 3;40,27,39;,
 3;40,28,27;,
 3;30,41,29;,
 3;29,41,53;,
 3;29,53,43;,
 3;43,53,41;,
 3;44,28,40;,
 3;30,45,41;,
 3;29,43,46;,
 3;32,48,47;,
 3;49,33,32;,
 3;49,32,47;,
 3;29,46,34;,
 3;35,48,32;,
 3;20,36,50;,
 3;38,37,51;,
 3;38,51,52;,
 3;50,36,38;,
 3;50,38,52;;
 
 MeshMaterialList {
  2;
  80;
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
  1,
  1,
  1,
  0,
  1,
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
  1,
  1,
  1,
  0,
  1,
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
  1,
  1,
  0,
  0,
  1,
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
  1,
  1,
  1,
  0,
  1,
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
    "gladius.jpg";
   }
  }
  Material {
   0.269804;0.141176;0.053333;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  85;
  0.000000;-0.998459;-0.055503;,
  0.166490;-0.985552;-0.031100;,
  0.000000;-0.999308;-0.037187;,
  0.282280;-0.958741;-0.033674;,
  0.000000;-0.963160;-0.268928;,
  0.202720;-0.863753;-0.461342;,
  0.000000;-0.953710;-0.300728;,
  0.590721;-0.794896;-0.138525;,
  0.395076;-0.751357;0.528561;,
  0.254354;-0.951033;-0.175614;,
  0.490688;-0.829721;0.266060;,
  0.654897;-0.753109;-0.062741;,
  0.707041;-0.706697;-0.025928;,
  0.643431;-0.636105;-0.425872;,
  0.643436;-0.636105;-0.425864;,
  0.517017;-0.848386;0.113725;,
  0.620367;-0.611899;-0.490637;,
  0.660588;-0.654259;-0.368196;,
  0.241135;-0.584269;0.774909;,
  0.608314;-0.670308;0.425019;,
  -0.000000;-0.017455;0.999848;,
  0.000000;0.999997;-0.002289;,
  0.000000;0.971960;-0.235148;,
  -0.000000;0.963625;-0.267260;,
  0.590719;0.799247;-0.110702;,
  0.654890;0.754846;-0.036425;,
  0.643407;0.650600;-0.403419;,
  0.517015;0.843902;0.143266;,
  0.620363;0.628657;-0.468978;,
  0.241133;0.556869;0.794828;,
  -0.166490;-0.985553;-0.031100;,
  -0.282280;-0.958741;-0.033674;,
  -0.202720;-0.863753;-0.461341;,
  -0.590721;-0.794896;-0.138526;,
  -0.395076;-0.751357;0.528561;,
  -0.254354;-0.951033;-0.175614;,
  -0.490688;-0.829721;0.266060;,
  -0.707041;-0.706697;-0.025928;,
  -0.643437;-0.636105;-0.425864;,
  -0.660589;-0.654259;-0.368196;,
  -0.608315;-0.670307;0.425019;,
  -0.590719;0.799247;-0.110703;,
  -0.000000;-0.776407;0.630231;,
  0.172667;-0.823495;0.540410;,
  0.814823;-0.538939;0.213561;,
  0.624247;-0.726622;0.286942;,
  0.166488;0.986038;0.003314;,
  0.000000;0.999787;-0.020624;,
  0.282278;0.959333;-0.000194;,
  0.202720;0.879328;-0.430915;,
  0.395077;0.732455;0.554458;,
  0.172669;0.804137;0.568814;,
  -0.000000;0.753942;0.656941;,
  0.814821;0.531161;0.232239;,
  0.254352;0.956583;-0.142315;,
  0.490686;0.819931;0.294856;,
  0.707041;0.707172;-0.001258;,
  0.643428;0.650593;-0.403398;,
  0.608316;0.655073;0.448141;,
  0.660591;0.666713;-0.345128;,
  0.624242;0.716173;0.312120;,
  -0.172667;-0.823495;0.540409;,
  -0.814822;-0.538940;0.213560;,
  -0.654897;-0.753109;-0.062741;,
  -0.643431;-0.636105;-0.425872;,
  -0.517017;-0.848386;0.113725;,
  -0.620367;-0.611899;-0.490637;,
  -0.624247;-0.726622;0.286942;,
  -0.166488;0.986038;0.003314;,
  -0.282278;0.959333;-0.000194;,
  -0.202720;0.879328;-0.430915;,
  -0.172669;0.804137;0.568814;,
  -0.395076;0.732455;0.554458;,
  -0.814820;0.531162;0.232239;,
  -0.254352;0.956583;-0.142315;,
  -0.490686;0.819931;0.294856;,
  -0.654890;0.754846;-0.036425;,
  -0.707041;0.707172;-0.001258;,
  -0.643428;0.650593;-0.403397;,
  -0.643407;0.650600;-0.403419;,
  -0.517015;0.843902;0.143266;,
  -0.608317;0.655073;0.448141;,
  -0.620363;0.628657;-0.468978;,
  -0.660591;0.666713;-0.345128;,
  -0.624242;0.716172;0.312120;;
  80;
  3;0,1,2;,
  3;3,2,1;,
  3;3,4,2;,
  3;4,5,6;,
  3;6,5,7;,
  3;42,43,8;,
  3;8,7,44;,
  3;9,4,3;,
  3;4,9,5;,
  3;42,8,10;,
  3;11,10,12;,
  3;14,13,11;,
  3;14,11,12;,
  3;6,10,15;,
  3;15,10,11;,
  3;20,18,19;,
  3;16,13,14;,
  3;16,14,17;,
  3;19,18,45;,
  3;19,16,17;,
  3;21,46,47;,
  3;46,21,48;,
  3;21,22,48;,
  3;23,49,22;,
  3;24,49,23;,
  3;50,51,52;,
  3;53,24,50;,
  3;48,22,54;,
  3;49,54,22;,
  3;55,50,52;,
  3;56,55,25;,
  3;25,26,57;,
  3;56,25,57;,
  3;27,55,23;,
  3;25,55,27;,
  3;58,29,20;,
  3;57,26,28;,
  3;59,57,28;,
  3;60,29,58;,
  3;59,28,58;,
  3;2,30,0;,
  3;30,2,31;,
  3;2,4,31;,
  3;6,32,4;,
  3;33,32,6;,
  3;34,61,42;,
  3;62,33,34;,
  3;31,4,35;,
  3;32,35,4;,
  3;36,34,42;,
  3;37,36,63;,
  3;63,64,38;,
  3;37,63,38;,
  3;65,36,6;,
  3;63,36,65;,
  3;40,18,20;,
  3;38,64,66;,
  3;39,38,66;,
  3;67,67,40;,
  3;39,66,40;,
  3;47,68,21;,
  3;69,21,68;,
  3;69,22,21;,
  3;22,70,23;,
  3;23,70,41;,
  3;52,71,72;,
  3;72,41,73;,
  3;74,22,69;,
  3;22,74,70;,
  3;52,72,75;,
  3;76,75,77;,
  3;78,79,76;,
  3;78,76,77;,
  3;23,75,80;,
  3;80,75,76;,
  3;20,29,81;,
  3;82,79,78;,
  3;82,78,83;,
  3;81,84,84;,
  3;81,82,83;;
 }
 MeshTextureCoords {
  54;
  0.025590;0.025590;,
  0.104230;0.242870;,
  0.025590;0.451250;,
  0.088620;0.593580;,
  0.025590;0.921550;,
  0.241240;0.104660;,
  0.399240;0.111540;,
  0.241840;0.134140;,
  0.382580;0.135130;,
  0.393600;0.160550;,
  0.117930;0.883210;,
  0.333570;0.066330;,
  0.296860;0.175500;,
  0.244820;0.383660;,
  0.293300;0.264660;,
  0.280760;0.383660;,
  0.297850;0.502260;,
  0.244820;0.502260;,
  0.241170;0.195160;,
  0.244820;0.277400;,
  0.254140;0.677760;,
  0.253710;0.629240;,
  0.368910;0.629660;,
  0.254250;0.594990;,
  0.253950;0.572900;,
  0.349050;0.572900;,
  0.369590;0.594990;,
  0.025590;0.451250;,
  0.025590;0.921550;,
  0.241840;0.134140;,
  0.241240;0.104660;,
  0.382580;0.135130;,
  0.244820;0.383660;,
  0.244820;0.502260;,
  0.241170;0.195160;,
  0.244820;0.277400;,
  0.253710;0.629240;,
  0.253950;0.572900;,
  0.254250;0.594990;,
  0.104230;0.242870;,
  0.088620;0.593580;,
  0.399240;0.111540;,
  0.382580;0.135130;,
  0.393600;0.160550;,
  0.117930;0.883210;,
  0.333570;0.066330;,
  0.296860;0.175500;,
  0.280760;0.383660;,
  0.293300;0.264660;,
  0.297850;0.502260;,
  0.368910;0.629660;,
  0.349050;0.572900;,
  0.369590;0.594990;,
  0.382580;0.135130;;
 }
}

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
 76;
 -2.63472;0.35327;-3.21531;,
 -1.73507;0.35327;-4.44499;,
 -1.74384;-4.21740;-4.46794;,
 -2.64799;-4.21740;-3.23517;,
 0.01994;0.35327;-4.44499;,
 0.01994;-4.21740;-4.46794;,
 1.77495;0.35327;-4.44499;,
 1.78372;-4.21740;-4.46794;,
 2.52217;0.35327;-3.40361;,
 2.53544;-4.21740;-3.42348;,
 -1.67485;-10.74346;-4.28751;,
 -2.54363;-10.74346;-3.68970;,
 0.01994;-10.74346;-4.28751;,
 1.71472;-10.74346;-4.28751;,
 2.58352;-10.74346;-3.68970;,
 3.52996;0.35327;-2.15006;,
 3.54751;-4.21740;-2.16153;,
 3.52996;0.35327;0.14487;,
 3.54751;-4.21740;0.14487;,
 3.52996;0.35327;2.43980;,
 3.54751;-4.21740;2.45128;,
 2.67460;0.35327;3.54499;,
 2.68787;-4.21740;3.56198;,
 3.40952;-10.74346;-2.07132;,
 3.40952;-10.74346;0.14487;,
 3.40952;-10.74346;2.36106;,
 2.58352;-10.74346;3.42833;,
 1.77495;0.35327;4.73471;,
 1.78372;-4.21740;4.75768;,
 0.01994;0.35327;4.73471;,
 0.01994;-4.21740;4.75768;,
 -1.73507;0.35327;4.73471;,
 -1.74384;-4.21740;4.75768;,
 -2.63472;0.35327;3.54499;,
 -2.64799;-4.21740;3.56198;,
 1.71472;-10.74346;4.57725;,
 0.01994;-10.74346;4.57725;,
 -1.67485;-10.74346;4.57725;,
 -2.54363;-10.74346;3.42833;,
 -3.49007;0.35327;2.43980;,
 -3.50762;-4.21740;2.45128;,
 -3.49007;0.35327;0.14487;,
 -3.50762;-4.21740;0.14487;,
 -3.49007;0.35327;-2.15006;,
 -3.50762;-4.21740;-2.16153;,
 -3.36963;-10.74346;2.36106;,
 -3.36963;-10.74346;0.14487;,
 -3.36963;-10.74346;-2.07132;,
 -1.73507;0.35327;2.43980;,
 0.01994;0.35327;2.43980;,
 1.77495;0.35327;2.43980;,
 -1.73507;0.35327;0.14487;,
 0.01994;0.35327;0.14487;,
 1.77495;0.35327;0.14487;,
 -1.73507;0.35327;-2.15006;,
 0.01994;0.35327;-2.15006;,
 1.77495;0.35327;-2.15006;,
 -2.47404;-15.63341;-3.42428;,
 -1.62882;-15.63367;-3.91280;,
 0.01996;-15.63362;-3.91277;,
 1.66869;-15.63372;-3.91272;,
 2.51387;-15.63341;-3.42417;,
 3.31740;-15.63243;-1.75681;,
 3.31740;-15.63115;0.39904;,
 3.31739;-15.62988;2.55490;,
 2.51386;-15.62932;3.50023;,
 1.66869;-15.62860;4.71082;,
 0.01996;-15.62859;4.71089;,
 -1.62881;-15.62859;4.71094;,
 -2.47403;-15.62932;3.50036;,
 -3.27763;-15.62986;2.55507;,
 -3.27763;-15.63115;0.39909;,
 -3.27763;-15.63243;-1.75687;,
 -1.62881;-15.62859;4.71094;,
 3.31740;-15.63115;0.39904;,
 0.01996;-15.63362;-3.91277;;
 
 71;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;3,2,10,11;,
 4;2,5,12,10;,
 4;5,7,13,12;,
 4;7,9,14,13;,
 4;8,15,16,9;,
 4;15,17,18,16;,
 4;17,19,20,18;,
 4;19,21,22,20;,
 4;9,16,23,14;,
 4;16,18,24,23;,
 4;18,20,25,24;,
 4;20,22,26,25;,
 4;21,27,28,22;,
 4;27,29,30,28;,
 4;29,31,32,30;,
 4;31,33,34,32;,
 4;22,28,35,26;,
 4;28,30,36,35;,
 4;30,32,37,36;,
 4;32,34,38,37;,
 4;33,39,40,34;,
 4;39,41,42,40;,
 4;41,43,44,42;,
 4;43,0,3,44;,
 4;34,40,45,38;,
 4;40,42,46,45;,
 4;42,44,47,46;,
 4;44,3,11,47;,
 4;33,31,48,39;,
 4;31,29,49,48;,
 4;29,27,50,49;,
 4;27,21,19,50;,
 4;39,48,51,41;,
 4;48,49,52,51;,
 4;49,50,53,52;,
 4;50,19,17,53;,
 4;41,51,54,43;,
 4;51,52,55,54;,
 4;52,53,56,55;,
 4;53,17,15,56;,
 4;43,54,1,0;,
 4;54,55,4,1;,
 4;55,56,6,4;,
 4;56,15,8,6;,
 4;57,11,10,58;,
 4;58,10,12,59;,
 4;59,12,13,60;,
 4;60,13,14,61;,
 4;61,14,23,62;,
 4;62,23,24,63;,
 4;63,24,25,64;,
 4;64,25,26,65;,
 4;65,26,35,66;,
 4;66,35,36,67;,
 4;67,36,37,68;,
 4;68,37,38,69;,
 4;69,38,45,70;,
 4;70,45,46,71;,
 4;71,46,47,72;,
 4;72,47,11,57;,
 3;73,74,75;,
 4;57,58,60,61;,
 4;72,57,61,62;,
 3;62,71,72;,
 3;62,63,71;,
 4;70,71,63,64;,
 4;64,66,68,70;;
 
 MeshMaterialList {
  1;
  71;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\armor.jpg";
   }
  }
 }
 MeshNormals {
  74;
  -0.793628;0.004744;-0.608385;,
  -0.452245;0.005457;-0.891877;,
  0.000000;0.005021;-0.999987;,
  0.456300;0.005458;-0.889809;,
  -0.783535;0.001346;-0.621347;,
  -0.413651;-0.004993;-0.910422;,
  0.000000;-0.011310;-0.999936;,
  0.416693;-0.004796;-0.909035;,
  -0.758068;-0.027213;-0.651607;,
  -0.326253;-0.043919;-0.944262;,
  0.000006;-0.052038;-0.998645;,
  0.327282;-0.043745;-0.943914;,
  0.796176;0.004735;-0.605046;,
  0.943319;0.004566;-0.331856;,
  0.999993;0.003840;0.000000;,
  0.946251;0.004445;0.323404;,
  0.785466;0.002502;-0.618900;,
  0.951585;-0.003298;-0.307367;,
  0.999963;-0.008651;-0.000000;,
  0.946215;-0.010014;0.323383;,
  0.758772;-0.026038;-0.650836;,
  0.966593;-0.020992;-0.255454;,
  0.999800;-0.019992;0.000003;,
  0.944269;-0.016187;0.328777;,
  0.794226;0.004566;0.607605;,
  0.445444;0.005353;0.895294;,
  0.000000;0.005026;0.999987;,
  -0.445444;0.005353;0.895294;,
  0.794193;-0.010286;0.607579;,
  0.445425;-0.012051;0.895238;,
  0.000000;-0.011307;0.999936;,
  -0.445426;-0.012051;0.895238;,
  0.793590;-0.010888;0.608356;,
  0.449728;-0.006351;0.893143;,
  0.000009;-0.000146;1.000000;,
  -0.449719;-0.006342;0.893148;,
  -0.794228;0.004566;0.607603;,
  -0.946252;0.004445;0.323401;,
  -0.999993;0.003840;0.000000;,
  -0.943436;0.004565;-0.331524;,
  -0.794195;-0.010287;0.607576;,
  -0.946216;-0.010014;0.323381;,
  -0.999963;-0.008651;0.000000;,
  -0.951667;-0.004314;-0.307101;,
  -0.793585;-0.010874;0.608363;,
  -0.944267;-0.016171;0.328784;,
  -0.999800;-0.019979;0.000003;,
  -0.966598;-0.021978;-0.255352;,
  0.000000;1.000000;0.000000;,
  -0.741576;-0.053119;-0.668763;,
  -0.277403;-0.071621;-0.958080;,
  0.000011;-0.076408;-0.997077;,
  0.277423;-0.071635;-0.958073;,
  0.741585;-0.053142;-0.668751;,
  0.973167;-0.030786;-0.228029;,
  0.999822;-0.018844;0.000007;,
  0.942452;-0.007867;0.334249;,
  0.793033;0.003368;0.609169;,
  0.453955;0.016865;0.890865;,
  0.000018;0.027345;0.999626;,
  -0.453934;0.016883;0.890876;,
  -0.793020;0.003396;0.609186;,
  -0.942447;-0.007835;0.334263;,
  -0.999823;-0.018819;0.000006;,
  -0.973167;-0.030761;-0.228033;,
  0.000011;1.000000;-0.000581;,
  -0.000004;-1.000000;0.000579;,
  -0.000008;-1.000000;0.000569;,
  -0.000000;-1.000000;0.000591;,
  -0.000000;-1.000000;0.000592;,
  -0.000000;-1.000000;0.000594;,
  -0.000001;-1.000000;0.000594;,
  -0.000002;-1.000000;0.000592;,
  -0.000003;-1.000000;0.000591;;
  71;
  4;0,1,5,4;,
  4;1,2,6,5;,
  4;2,3,7,6;,
  4;3,12,16,7;,
  4;4,5,9,8;,
  4;5,6,10,9;,
  4;6,7,11,10;,
  4;7,16,20,11;,
  4;12,13,17,16;,
  4;13,14,18,17;,
  4;14,15,19,18;,
  4;15,24,28,19;,
  4;16,17,21,20;,
  4;17,18,22,21;,
  4;18,19,23,22;,
  4;19,28,32,23;,
  4;24,25,29,28;,
  4;25,26,30,29;,
  4;26,27,31,30;,
  4;27,36,40,31;,
  4;28,29,33,32;,
  4;29,30,34,33;,
  4;30,31,35,34;,
  4;31,40,44,35;,
  4;36,37,41,40;,
  4;37,38,42,41;,
  4;38,39,43,42;,
  4;39,0,4,43;,
  4;40,41,45,44;,
  4;41,42,46,45;,
  4;42,43,47,46;,
  4;43,4,8,47;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;48,48,48,48;,
  4;49,8,9,50;,
  4;50,9,10,51;,
  4;51,10,11,52;,
  4;52,11,20,53;,
  4;53,20,21,54;,
  4;54,21,22,55;,
  4;55,22,23,56;,
  4;56,23,32,57;,
  4;57,32,33,58;,
  4;58,33,34,59;,
  4;59,34,35,60;,
  4;60,35,44,61;,
  4;61,44,45,62;,
  4;62,45,46,63;,
  4;63,46,47,64;,
  4;64,47,8,49;,
  3;65,65,65;,
  4;66,67,67,66;,
  4;68,66,66,69;,
  3;69,70,68;,
  3;69,71,70;,
  4;72,70,71,72;,
  4;72,73,73,72;;
 }
 MeshTextureCoords {
  76;
  0.225884;0.197506;,
  0.270867;0.197506;,
  0.270428;0.426040;,
  0.225221;0.426040;,
  0.358617;0.197506;,
  0.358617;0.426040;,
  0.446368;0.197506;,
  0.446806;0.426040;,
  0.483728;0.197506;,
  0.484392;0.426040;,
  0.273878;0.752343;,
  0.230439;0.752343;,
  0.358617;0.752343;,
  0.443356;0.752343;,
  0.486796;0.752343;,
  0.534118;0.197506;,
  0.534995;0.426040;,
  0.534118;0.197506;,
  0.534995;0.426040;,
  0.534118;0.197506;,
  0.534995;0.426040;,
  0.491350;0.197506;,
  0.492014;0.426040;,
  0.528096;0.752343;,
  0.528096;0.752343;,
  0.528096;0.752343;,
  0.486796;0.752343;,
  0.446368;0.197506;,
  0.446806;0.426040;,
  0.358617;0.197506;,
  0.358617;0.426040;,
  0.270867;0.197506;,
  0.270428;0.426040;,
  0.225884;0.197506;,
  0.225221;0.426040;,
  0.443356;0.752343;,
  0.358617;0.752343;,
  0.273878;0.752343;,
  0.230439;0.752343;,
  0.183116;0.197506;,
  0.182239;0.426040;,
  0.183116;0.197506;,
  0.182239;0.426040;,
  0.183116;0.197506;,
  0.182239;0.426040;,
  0.189139;0.752343;,
  0.189139;0.752343;,
  0.189139;0.752343;,
  0.270867;0.197506;,
  0.358617;0.197506;,
  0.446368;0.197506;,
  0.270867;0.197506;,
  0.358617;0.197506;,
  0.446368;0.197506;,
  0.270867;0.197506;,
  0.358617;0.197506;,
  0.446368;0.197506;,
  0.233918;0.996840;,
  0.276179;0.996853;,
  0.358618;0.996851;,
  0.441054;0.996856;,
  0.483314;0.996840;,
  0.523490;0.996791;,
  0.523490;0.996727;,
  0.523489;0.996664;,
  0.483313;0.996636;,
  0.441054;0.996600;,
  0.358618;0.996599;,
  0.276179;0.996599;,
  0.233919;0.996636;,
  0.193739;0.996663;,
  0.193739;0.996727;,
  0.193739;0.996791;,
  0.276179;0.996599;,
  0.523490;0.996727;,
  0.358618;0.996851;;
 }
}

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
 97;
 -10.51896;-4.67970;-1.16692;,
 -9.54484;-4.67970;5.36901;,
 -7.54976;0.08676;3.96162;,
 -8.32567;0.08676;-1.24422;,
 0.25907;0.08676;6.02858;,
 0.25907;-6.22643;7.96404;,
 10.06294;-4.67970;5.36901;,
 11.03709;-4.67970;-1.16692;,
 10.06294;-4.67970;-5.68648;,
 8.06786;0.08676;-4.84408;,
 8.84379;0.08676;-1.24422;,
 0.25907;0.08676;-6.10380;,
 0.25907;-12.62775;-7.26804;,
 -4.64287;-4.67970;-6.59789;,
 -3.64536;0.08676;-5.57002;,
 4.16346;0.08676;-5.57002;,
 6.74776;-4.67970;-6.30287;,
 5.16100;-4.67970;-6.59789;,
 -9.54484;-4.67970;-5.68648;,
 -7.54976;0.08676;-4.84408;,
 4.22509;-4.75118;-6.72586;,
 8.06786;0.08676;3.96162;,
 6.47211;-11.63451;-8.15718;,
 9.47517;-15.88677;-9.04141;,
 12.53195;-16.51221;-1.16692;,
 11.55780;-16.51221;-5.68648;,
 11.55780;-16.51221;5.36901;,
 -12.08800;-18.07937;-1.16692;,
 -11.11384;-18.07937;5.36897;,
 -11.11384;-18.07937;-5.68648;,
 -12.48684;-15.71595;6.96439;,
 -4.31307;-17.21239;9.11405;,
 9.32158;-15.50810;7.63217;,
 3.57349;-15.34606;8.89222;,
 8.29661;-16.65528;8.08021;,
 0.02627;-18.56630;9.55943;,
 4.36560;-17.52559;9.11405;,
 -7.28164;-17.40326;-6.53074;,
 11.08995;-15.88677;-9.04141;,
 7.86022;-15.88677;-9.04141;,
 6.89242;-15.88677;-9.04141;,
 5.69274;-13.71296;-8.58938;,
 4.87761;-15.88677;-9.04141;,
 2.79261;-15.88677;-9.04141;,
 5.20432;-12.82333;-9.38897;,
 4.87761;-18.91752;-9.04141;,
 7.52360;-19.84867;-9.68337;,
 8.15238;-21.68182;-9.89601;,
 8.38352;-20.32182;-9.56344;,
 9.47517;-19.01542;-9.04141;,
 9.33561;-20.22712;-9.32109;,
 10.01969;-19.07459;-8.91692;,
 8.57881;-19.42718;-9.04141;,
 11.67160;-18.04453;-9.33143;,
 2.79261;-17.02695;-9.04141;,
 2.78572;-18.08465;-9.48864;,
 3.10426;-17.32917;-9.14718;,
 4.27772;-16.90044;-9.51163;,
 3.68337;-16.83703;-9.52656;,
 4.08642;-17.95636;-9.97105;,
 5.65164;-18.91759;-8.98714;,
 5.15497;-19.13825;-9.11516;,
 4.74217;-19.57963;-9.33058;,
 -3.93491;-16.81280;-7.26804;,
 -6.67975;-18.03942;-6.99611;,
 -3.90247;-18.47519;-8.11315;,
 -10.84554;-19.90108;-6.58344;,
 -4.24130;-19.48704;-8.65974;,
 -4.25352;-20.16161;-9.00499;,
 -5.16170;-19.92063;-8.96362;,
 -6.74022;-19.63435;-7.81516;,
 -6.36049;-19.23566;-7.57769;,
 -5.72147;-19.68428;-7.74912;,
 -6.76417;-20.08294;-8.04615;,
 -7.19683;-19.31037;-7.69089;,
 4.91855;-19.11282;9.58538;,
 5.61623;-18.61151;9.31902;,
 4.68634;-20.33812;10.03801;,
 4.13372;-19.80906;9.93051;,
 -0.46025;-19.63876;9.99377;,
 -1.94593;-21.21931;10.73737;,
 -1.87788;-21.64265;10.87376;,
 -0.61371;-20.54188;10.32499;,
 -1.21857;-20.09040;10.25118;,
 6.82375;-18.24068;8.90774;,
 6.67866;-17.01349;8.50573;,
 11.85618;-12.67132;6.52426;,
 10.47291;-16.17345;7.50786;,
 12.53937;-15.71595;6.96439;,
 6.81943;-18.76180;9.08767;,
 7.44507;-18.50959;8.91353;,
 11.09829;-19.91795;-8.77733;,
 11.30830;-21.06267;-8.41274;,
 8.40668;-17.75172;8.44213;,
 9.04294;-18.29144;8.53903;,
 8.40954;-18.42648;8.67390;,
 8.04701;-18.13972;8.62589;;
 
 87;
 4;0,1,2,3;,
 3;4,5,6;,
 4;7,8,9,10;,
 3;11,12,13;,
 3;3,11,14;,
 3;11,10,15;,
 3;15,16,17;,
 4;1,5,4,2;,
 3;13,18,19;,
 3;11,20,12;,
 4;6,7,10,21;,
 4;18,0,3,19;,
 3;22,16,23;,
 4;8,7,24,25;,
 4;6,26,24,7;,
 4;1,0,27,28;,
 4;18,29,27,0;,
 4;1,30,31,5;,
 3;32,33,34;,
 3;31,35,5;,
 4;35,36,33,5;,
 3;37,29,12;,
 4;16,8,38,23;,
 4;39,40,41,22;,
 4;42,43,44,41;,
 3;40,42,41;,
 3;44,20,41;,
 3;40,45,42;,
 3;39,46,40;,
 3;47,46,48;,
 3;23,49,39;,
 3;50,49,51;,
 3;49,52,39;,
 3;38,53,23;,
 3;42,54,43;,
 3;55,54,56;,
 3;57,58,42;,
 3;57,59,58;,
 3;60,61,45;,
 3;45,61,62;,
 3;63,64,37;,
 3;65,64,63;,
 4;64,66,29,37;,
 3;64,65,67;,
 3;68,69,67;,
 4;70,71,72,73;,
 3;71,74,64;,
 3;71,70,74;,
 3;75,76,36;,
 3;77,75,78;,
 3;35,31,79;,
 3;80,81,82;,
 3;82,79,83;,
 3;36,84,85;,
 4;86,32,87,88;,
 3;87,32,34;,
 3;85,33,36;,
 3;89,90,84;,
 3;91,92,51;,
 3;93,94,34;,
 3;95,93,96;,
 3;4,6,21;,
 3;11,13,14;,
 3;11,3,4;,
 3;2,4,3;,
 3;19,3,14;,
 3;10,4,21;,
 3;4,10,11;,
 3;15,10,9;,
 3;16,9,8;,
 3;9,16,15;,
 3;13,19,14;,
 3;20,15,17;,
 3;15,20,11;,
 3;16,20,17;,
 3;22,23,39;,
 3;16,22,20;,
 3;5,86,6;,
 3;86,33,32;,
 3;34,33,85;,
 3;33,86,5;,
 3;18,12,29;,
 3;37,12,63;,
 3;12,18,13;,
 3;12,44,43;,
 3;41,20,22;,
 3;20,44,12;;
 
 MeshMaterialList {
  1;
  87;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.056471;0.056471;0.056471;1.000000;;
   30.000000;
   0.140000;0.140000;0.140000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  99;
  -0.169842;0.077241;-0.982440;,
  -0.177326;0.242781;-0.953736;,
  -0.952660;0.268846;0.141988;,
  -0.899410;0.416042;0.134052;,
  0.941776;0.268051;-0.202993;,
  0.892645;0.407634;-0.192404;,
  0.222051;0.281117;0.933631;,
  0.899410;0.416042;0.134056;,
  0.031663;0.206797;0.977871;,
  0.076348;0.330553;0.940694;,
  -0.205032;0.141786;-0.968431;,
  -0.008335;0.163544;-0.986501;,
  0.044297;0.208121;-0.977099;,
  -0.045779;0.022437;-0.998700;,
  0.000000;1.000000;0.000000;,
  -0.132645;0.157834;-0.978516;,
  0.160069;-0.021538;-0.986871;,
  0.961017;0.274734;-0.031108;,
  0.908222;0.417443;-0.029571;,
  -0.962254;0.270363;-0.031146;,
  -0.908222;0.417443;-0.029570;,
  0.970179;0.122568;-0.209114;,
  0.981442;0.123991;0.146280;,
  0.991608;0.125275;-0.031926;,
  -0.971209;0.113723;-0.209333;,
  -0.982507;0.115046;0.146437;,
  -0.992707;0.116240;-0.031958;,
  -0.219155;0.194236;0.956161;,
  -0.087953;0.232777;0.968545;,
  0.211157;0.186101;0.959572;,
  0.146818;0.238753;0.959918;,
  0.009820;0.209561;0.977746;,
  -0.264785;0.360110;-0.894544;,
  0.033021;0.175518;-0.983922;,
  0.149473;0.145586;-0.977989;,
  0.000000;0.113934;-0.993488;,
  0.020463;0.109014;-0.993830;,
  0.064044;0.094007;-0.993509;,
  0.045933;0.223239;-0.973681;,
  0.000000;0.142286;-0.989826;,
  0.063207;0.121655;-0.990558;,
  0.673876;0.049219;-0.737203;,
  0.043813;0.263594;-0.963638;,
  0.120154;0.177408;-0.976775;,
  0.238523;0.192236;-0.951920;,
  0.080373;0.064777;-0.994658;,
  0.238529;0.192244;-0.951917;,
  0.056736;-0.068713;-0.996022;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.133211;-0.991088;,
  0.032941;0.198180;-0.979612;,
  0.064538;0.388279;-0.919279;,
  0.064506;0.388271;-0.919285;,
  0.064507;0.388279;-0.919282;,
  0.064565;0.388273;-0.919280;,
  0.064531;0.388236;-0.919298;,
  0.064497;0.388199;-0.919316;,
  -0.263540;0.394592;-0.880252;,
  -0.262253;0.477776;-0.838423;,
  -0.257671;0.323660;-0.910412;,
  -0.196378;0.444140;-0.874171;,
  -0.084402;0.495338;-0.864590;,
  0.079752;0.453009;-0.887931;,
  0.079734;0.452963;-0.887957;,
  0.079741;0.452958;-0.887959;,
  0.079761;0.452953;-0.887959;,
  0.079720;0.452968;-0.887956;,
  0.126344;0.322582;0.938071;,
  0.126352;0.322579;0.938071;,
  0.126337;0.322584;0.938072;,
  0.072936;0.345760;0.935484;,
  0.126327;0.322569;0.938078;,
  0.126350;0.322556;0.938080;,
  0.126304;0.322583;0.938077;,
  0.216854;0.183349;0.958831;,
  0.193326;0.217165;0.956799;,
  0.213300;0.184152;0.959474;,
  0.211157;0.186105;0.959572;,
  0.200665;0.225987;0.953238;,
  0.191523;0.170079;0.966640;,
  0.151767;0.323870;0.933850;,
  0.130880;0.322593;0.937446;,
  -0.130906;-0.322586;-0.937445;,
  0.130916;0.322568;0.937449;,
  0.130849;0.322573;0.937457;,
  0.130984;0.322564;0.937441;,
  0.150249;0.237530;-0.959690;,
  -0.218408;0.256234;0.941617;,
  -0.216774;0.317193;0.923254;,
  0.951452;0.273182;0.141811;,
  -0.942935;0.263758;-0.203238;,
  -0.892646;0.407635;-0.192399;,
  -0.223100;0.224085;-0.948690;,
  0.132945;0.262408;-0.955755;,
  0.921770;0.217679;-0.320869;,
  0.238189;0.365398;0.899861;,
  -0.154533;0.239065;-0.958628;,
  0.177327;0.242783;-0.953736;,
  -0.376762;0.223091;-0.899044;;
  87;
  4;19,2,3,20;,
  3;9,8,6;,
  4;17,4,5,18;,
  3;11,10,15;,
  3;14,14,14;,
  3;14,14,14;,
  3;86,34,16;,
  4;87,8,9,88;,
  3;15,0,1;,
  3;11,36,10;,
  4;89,17,18,7;,
  4;90,19,20,91;,
  3;38,34,33;,
  4;4,17,23,21;,
  4;89,22,23,17;,
  4;2,19,26,25;,
  4;90,24,26,19;,
  4;87,27,28,8;,
  3;76,79,75;,
  3;28,31,8;,
  4;31,30,79,8;,
  3;59,92,10;,
  4;34,93,12,33;,
  4;35,39,40,38;,
  4;37,13,41,40;,
  3;39,37,40;,
  3;41,94,94;,
  3;39,42,37;,
  3;35,43,39;,
  3;44,43,44;,
  3;33,45,35;,
  3;46,45,47;,
  3;45,48,35;,
  3;12,49,33;,
  3;37,50,13;,
  3;51,50,51;,
  3;52,52,37;,
  3;52,53,52;,
  3;56,55,42;,
  3;42,55,54;,
  3;32,60,59;,
  3;58,60,32;,
  4;60,57,92,59;,
  3;60,58,61;,
  3;62,62,61;,
  4;64,63,65,65;,
  3;63,66,60;,
  3;63,64,66;,
  3;67,68,30;,
  3;69,67,69;,
  3;31,28,70;,
  3;72,72,71;,
  3;71,70,73;,
  3;30,80,78;,
  4;74,76,77,29;,
  3;77,76,75;,
  3;78,79,30;,
  3;81,81,80;,
  3;82,82,47;,
  3;83,84,75;,
  3;85,83,85;,
  3;9,6,95;,
  3;11,15,96;,
  3;14,14,14;,
  3;14,14,14;,
  3;14,14,14;,
  3;14,14,14;,
  3;14,14,14;,
  3;14,14,14;,
  3;34,97,93;,
  3;97,34,86;,
  3;15,1,96;,
  3;36,86,16;,
  3;86,36,11;,
  3;34,36,16;,
  3;38,33,35;,
  3;34,38,36;,
  3;8,74,6;,
  3;74,79,76;,
  3;75,79,78;,
  3;79,74,8;,
  3;0,10,92;,
  3;59,10,32;,
  3;10,0,15;,
  3;10,98,13;,
  3;40,36,38;,
  3;36,98,10;;
 }
 MeshTextureCoords {
  97;
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

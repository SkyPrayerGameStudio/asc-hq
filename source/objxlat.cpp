// This file is included by loadbi3.cpp and bi2pcx.cpp

// The first entry is the picture number that is going to be replaced by the picture number of the second entry.

const int terraintranslatenum = 17;
const int terraintranslate[terraintranslatenum][2] = {{ 574 , 526 } , { 575 , 1233 }, {577, 1244 }, 
                                                      { 581 ,1260 } , { 573 , 1226 }, {572, 1221 }, 
                                                      { 576 ,1238 } , { 578 , 1245 }, {579, 1249 }, 
                                                      { 580 ,1253 } , { 242 , 1135 }, {463,  449 },
                                                      { 464,  450 } , { 465,  451  }, {466,  452 },
                                                      { 237, 1110 } , { 233, 1094 }};


// This is a special translation for the fields that must be translated to a terrain AND an additional object

const int terraincombixlatnum = 2;
struct terraincombixlat {
           int bigraph;
           int terrainid;
           int terrainweather;
           int objectid;
       };
const terraincombixlat terraincombixlat[terraincombixlatnum] = {{ 222, 1011, 0, 1 }, 
                                                                { 223, 1012, 0, 1 }};




// These BI object pictures can be translated to up to four ASC objects. The first entry is again the BI picture number, 
//  the following four the pictures for the ASC objects. A -1 is used if the entry is not used. It does not matter if you 
//  use " -1, 1500, -1  , -1 " or " 1500, -1  , -1,  -1" or any other order.


const int objecttranslatenum = 35;
const int objecttranslate[objecttranslatenum][5] = {{ 1264, 1470, 1500, -1, -1 }, 
                                                    { 1265, 1470, -1, 1560, -1 }, 
                                                    { 1266, 1470, -1, -1, 1530 }, 
                                                    { 1267, -1, 1500, 1560, -1 }, 
                                                    { 1268, -1, 1500, -1, 1530 }, 
                                                    { 1269, -1, -1, 1560, 1530 }, 
                                                    { 1270, 1470, 1500, 1560, -1 }, 
                                                    { 1271, 1470, 1500, -1, 1530 }, 
                                                    { 1272, 1470, -1, 1560, 1530 }, 
                                                    { 1273, -1, 1500, 1560, 1530 }, 
                                                    { 1274, 1470, 1500, 1560, 1530 },
                                                    { 90  , 1470,  -1 , -1, -1   }, 
                                                    { 91  , -1  , -1, 1560, -1   }, 
                                                    { 92  , -1, 1500, -1  , -1   }, 
                                                    { 93  , -1  , -1  , -1  , 1530 },
                                                    { 459, 1190, -1, -1, -1 },
                                                    { 460, 1191, -1, -1, -1 },
                                                    { 1329, 1296, -1, -1, -1 },
                                                    { 1330, 1300, -1, -1, -1 },
                                                    { 1331, 1304, -1, -1, -1 },
                                                    {  234, 1098, -1, -1, -1 },
                                                    {  235, 1101, -1, -1, -1 },
                                                    {  236, 1102, -1, -1, -1 },
                                                    { 1283, 1284, -1, -1, -1 },
                                                    { 1334, 1325, -1, -1, -1 },
                                                    { 1333, 1320, -1, -1, -1 },
                                                    {  238, 1113, -1, -1, -1 },
                                                    {  239, 1118, -1, -1, -1 },
                                                    {  240, 1125, -1, -1, -1 },
                                                    {  241, 1131, -1, -1, -1 },
                                                    {  345, 1152, -1, -1, -1 },
                                                    { 1332, 1310, -1, -1, -1 },
                                                    {  347,  340, -1, -1, -1 },
                                                    {  348,  342, -1, -1, -1 },
                                                    {  461, 1203, -1, -1, -1 }
                                                    };

int getterraintranslatenum ( void )
{
   return terraintranslatenum;
}

int getobjectcontainertranslatenum ( void )
{
   return objecttranslatenum;
}

const int* getterraintranslate ( int pos )
{
   return terraintranslate[pos];
}

const int* getobjectcontainertranslate ( int pos )
{
   return objecttranslate[pos];
}

/*-- Neues Objekt --*/

#strict

static const ALTIMETER_CURSOR_OVERLAY=2;

local aMoreObjects;
local aSpecialPos;

func Initialize() {
// Fuer Parallaxitaet
Local(0)=0;
Local(1)=0;

// Initialisierung
aMoreObjects=[];
aSpecialPos=[];
return(1);
}

func DoInit(int iPlr)
{
 SetGraphics("Marker",this,GetID(),ALTIMETER_CURSOR_OVERLAY,GFXOV_MODE_Base,0,GFX_BLIT_Custom);
 SetClrModulation(GetPlrColorDw(iPlr),this,ALTIMETER_CURSOR_OVERLAY);
}

func Adjust()
{
 // Erst Cursor
 var cursor=GetCursor(GetOwner());
 if(!cursor)
 {
  SetObjDrawTransform(1,0,0,0,1,0,this,ALTIMETER_CURSOR_OVERLAY);
 }
 else
 {
  var y=GetRelativePosition(cursor);
  SetObjDrawTransform(1000,0,0,0,1000,y*1000-7*1000,this,ALTIMETER_CURSOR_OVERLAY);
 }
 
 // Dann noch zusaetzliche Objekte?
 
 for(var cnt=0;cnt<GetLength(aMoreObjects);cnt++)
 {
  var obj=aMoreObjects[cnt];
  if(!obj)
  {
   // Grafik loeschen
   SetGraphics(0,this,0,cnt+3,0,0,0,0);
   continue;
  }
  
  // Die Overlays sind sicher initialisiert, hoffentlich, wenn Overlayfehler dann zuerst hier gucken!
  var y=GetRelativePosition(obj);
  SetObjDrawTransform(1000,0,0,0,1000,y*1000+aSpecialPos[cnt],this,GetIndexOf(obj,aMoreObjects)+3);
 }
 
}

func AddSpecialObject(object pObj,int iClr,int iSpecialFeature)
{
 if(!pObj)return -1;
 var iOverlayMode=GFXOV_MODE_Base;
 var sGraphics=0;
 var idObj=GetID(pObj);
 if(iSpecialFeature == 1 && pObj != this)
 {
  iOverlayMode=GFXOV_MODE_Object;
  if(!iClr)iClr=RGB(255,255,255);
 }
 else
 if(iSpecialFeature == 2)
 {
  iOverlayMode=GFXOV_MODE_IngamePicture;
  if(!iClr)iClr=RGB(255,255,255);
 }
 else
 {
  sGraphics="Marker";
  idObj=GetID();
  if(!iClr)iClr=HSL(Random(255),200,50+Random(150));
 }
 // Leeres Element suchen
 var length=-1;
 
 for(var cnt=0;cnt<GetLength(aMoreObjects);cnt++)
 {
  if(aMoreObjects[cnt] != 0)continue;
  length=cnt;
  break;
 }
 
 if(length == -1)
  length=GetLength(aMoreObjects);
 aMoreObjects[length]=pObj;
 aSpecialPos[length]=-6*1000;
 
 if(iSpecialFeature == 1 && pObj != this)
  aSpecialPos[length]=-300*1000;
 
 SetGraphics(sGraphics,this,idObj,length+3,iOverlayMode,0,GFX_BLIT_Custom,pObj);
 SetClrModulation(iClr,this,length+3);
 return length+3; // Gibt die Overlaynummer zurueck. Wenn wer damit rumspielen muss..
}

func RemoveSpecialObject(pObj)
{
 var index;
 if(index=GetIndexOf(pObj,aMoreObjects) == -1) return -1;
 
 SetGraphics(0,this,0,index+3,0,0,0,0);
 
 aMoreObjects[index]=0;
}

// Fuegt irgendein Objekt zum Hoehenmesser hinzu
// Was special so kann, muss man selbst herausfinden :]
// Der Wert 2 ist verbugt. Der Wert 1 toll
global func AddAltimeterObject(int iPlayer,object pObj,int iClr,int special)
{
 var alti;
 if(!(alti=FindObject2(Find_ID(AM4Z),Find_Owner(iPlayer))))return -1;
 return alti->AddSpecialObject(pObj,iClr,special);
}

// Gegenstueck zu obiger Funktion
global func RemoveAltimeterObject(int iPlayer,object pObj)
{
 var alti;
 if(!(alti=FindObject2(Find_ID(AM4Z),Find_Owner(iPlayer))))return -1;
 return alti->RemoveSpecialObject(pObj);
}

func GetRelativePosition(object pObj)
{
 var height=LandscapeHeight();
 var y=BoundBy(GetY(pObj),0,height);;
 var bar=600;
 
 // Prozentrechnung :[
 var rel1=((y*100)/height);
 var rel2=((bar*rel1)/100);
 
 return rel2;
}

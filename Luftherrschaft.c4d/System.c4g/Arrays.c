/*-- Arrays --*/

#strict

//L�scht ein Item aus einem Array
global func DeleteArrayItem(iNumber, &aArray)
{
 var temp=[];
 for(var cnt;cnt<GetLength(aArray);cnt++)
 {
  if(cnt==iNumber)continue;
  var dif=0;
  if(cnt>iNumber)dif=-1;
  temp[cnt+dif]=aArray[cnt];
 } 
 
 aArray=temp;
 return(aArray);
}

//L�scht ein Item aus einem Array, kann m�glicherweise umsortieren
global func DeleteArrayItem2(iNumber,&aArray)
{
 //Ein ganz leeres Array?
 if(GetLength(aArray)==1)return((aArray=CreateArray()));
 //Wenn das letzte Element ist diese Funktion auch nciht toller.
 if(GetLength(aArray)-1==iNumber)
 return(DeleteArrayItem(iNumber, aArray));
 
 //Los!
 var last=aArray[GetLength(aArray)-1];
 aArray[GetLength(aArray)-1]=0;
 SetLength(aArray,GetLength(aArray)-1);
 aArray[iNumber]=last;
 return(aArray);
}

//Sucht ein Item im array
global func GetArrayItemPosition(value,aArray)
{
 var number=-1;
 for(var cnt=0;cnt<GetLength(aArray);cnt++)
 {
  if(aArray[cnt] == value)
  {
   number=cnt;
   break;
  }
  else continue;
 }
 
 return(number);
}

//F�gt ein Item am Ende eines Array ein
global func PushBack(value,&aArray)
{
 return(aArray[GetLength(aArray)]=value);
}

//F�gt ein Item am Anfang ein
global func PushFront(value,&aArray)
{
 var aNew=[];
 aNew[GetLength(aNew)]=value;
 for(var cnt=0;cnt<GetLength(aArray);cnt++)
 aNew[GetLength(aNew)]=aArray[cnt];
 
 aArray=aNew;
 return(1);
}

//F�hrt mit jedem Item im Array einen beliebigen Vergleich vor, der als String vorliegen sollte und gibt ein Array mit Ergebnissen zur�ck
// zB ForEach("<0",myArray);
//    ForEach("->~IsClonk()",myArray);
/*global func ForEach(sFunction,aArray)
{
 var aResults=[];
 
 Log("%d",GetLength(aArray));
 
 for(var cnt=0;cnt<GetLength(aArray);cnt++)
 {
  //var result=
 // eval(Format("Var(0)=(aArray[%d]%s)",cnt,sFunction));
 Var(0)=eval(Format("(%d%s)",aArray[cnt],sFunction));
  PushBack(Var(0),aResults);
 }
 
 return(aResults);
}

global func Test()
{
 var arr=[];
 arr[GetLength(arr)]=1;
 arr[GetLength(arr)]=20;
 arr[GetLength(arr)]=34;
 arr[GetLength(arr)]=21;
 arr[GetLength(arr)]=564;
 arr[GetLength(arr)]=13;
 arr[GetLength(arr)]=4;
 arr[GetLength(arr)]=76;
 arr[GetLength(arr)]=124;
 var res=ForEach("<150",arr);
 
 for(var integer in res)
 Log("%d",integer);
}*/









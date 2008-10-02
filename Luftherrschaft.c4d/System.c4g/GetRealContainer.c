#strict 2

global func GetRealContainer(object pOf) {
  var pCon = pOf;
  while(Contained(pCon)) pCon = Contained(pCon);
  return pCon;
}

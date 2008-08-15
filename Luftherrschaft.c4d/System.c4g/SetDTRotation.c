#strict

global func SetDTRotation (int r, int xoff, int yoff, object obj) {
  var fsin=Sin(r, 1000), fcos=Cos(r, 1000);
  // set matrix values
  SetObjDrawTransform (
    +fcos, +fsin, (1000-fcos)*xoff - fsin*yoff,
    -fsin, +fcos, (1000-fcos)*yoff + fsin*xoff,
    obj
  );
}

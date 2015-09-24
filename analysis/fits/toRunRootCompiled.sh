#!/bin/zsh

writeScript() {

    root -b<<EOF

.L HighMassGGFitterCompiled.cc++ 
runfits(750,"001")

.q
EOF

}

writeScript;

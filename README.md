# sciCalc
A simple, expandable user interface (UI) written in C for calculating various scientific values of use to the RF Hardware Engineer.

To make (update and recompile dependencies), type:

      make -f mc.mk

To run the main user interface, type:

      ./main
      
To update all automated tests at once, type: 

      make -f make-test.mk

To create automated Bash test scripts, run the following aliases in the linux CLI:

      alias ccsciCalc='cd ~/sciCalc-master; \
		make -f mc.mk; \
 		./main; \
		cd $OLDPWD'
		
      alias ccmaketest='cd ~/sciCalc-master; \
		make -f make-test.mk'

      alias cc1sciCalc='cd ~/sciCalc-master; \
		checkmk lbtest.check >lbtest.c; \
		gcc -g -o lbtest lbtest.c -lcheck -lm -lpthread -lrt -lsubunit -lcheck_pic; \
		./lbtest'

      alias cc2sciCalc='cd ~/sciCalc-master; \
		checkmk bpf2test.check >bpf2test.c; \
		gcc -g -o bpf2test bpf2test.c -lcheck -lm -lpthread -lrt -lsubunit -lcheck_pic; \
		./bpf2test'

      alias cc3sciCalc='cd ~/sciCalc-master; \
		checkmk inrushItest.check >inrushItest.c; \
		gcc -g -o inrushItest inrushItest.c -lcheck -lm -lpthread -lrt -lsubunit -lcheck_pic; \
		./inrushItest'

      alias cc4sciCalc='cd ~/sciCalc-master; \
		checkmk ustripZtest.check >ustripZtest.c; \
		gcc -g -o ustripZtest ustripZtest.c -lcheck -lm -lpthread -lrt -lsubunit -lcheck_pic; \
		./ustripZtest'


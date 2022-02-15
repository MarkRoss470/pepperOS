gdb bin/kernel.sym --eval-command="target remote localhost:6000"\
 --eval-command="b *0x7c00" --eval-command="c"

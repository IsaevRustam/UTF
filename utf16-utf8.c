#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE* Fin = stdin;
    FILE* Fout = stdout;
    char ch8;
    unsigned short utf16ch = 0;
    int checksymb, be = 0, numb = 0;
    if (argc > 1){
        Fin = fopen(argv[1],"r");
        if (argc == 3) Fout = fopen(argv[2],"w");
    }
    if((checksymb = fread(&utf16ch, sizeof utf16ch, 1, Fin)) == 0){
        if(feof(Fin) && utf16ch) fprintf(stderr, "Wrong(odd) numb of bytes, last is %X\n", utf16ch);
    fclose(Fin);
    fclose(Fout);
    return 0;
    }
    /* попытка сделать не масками не увенчалась успехом */
    unsigned short temp;
    be = utf16ch == 0xFFFE; // сравниваем сразу перед вайлом
    while((checksymb = fread(&utf16ch, sizeof utf16ch, 1, Fin)) != 0) {
        numb++;
        /*if (flag){
            be = utf16ch == 0xFEFF;
            flag = 0;
        }  */
        if(be){
            unsigned short sw = utf16ch & 0xFF00; // просто & на  1111 1111 0000 0000
            utf16ch <<=8; // свдигаем влево 
            sw >>=8; // сдвигаем вправо 
            utf16ch+=sw; // складываем и байты поменялись местами
        }
         /*utf16ch += ch8;
            if((ch8 & 0x80) != 0) {  // 1000 0000
                if((ch8 & 0xF0) == 0xE0) { 
                        numb++; */
        if((utf16ch & 0xF800) != 0) { // 1111 1000 0000 0000
            temp = utf16ch & 0xF000;
            temp >>= 12;
         //   b =temp >> 12;
          //  utf16ch &= 
            ch8 = 0xE0 + temp;  // 1110 0000
            fwrite(&ch8, sizeof ch8, 1, Fout);
            temp = utf16ch & 0x0FC0; // 0000 1111 1100
            temp >>= 6;
            ch8 = 0x80 + temp;  //  1000 0000 + temp к которому уже & 0fc0
            fwrite(&ch8, sizeof ch8, 1, Fout);
            temp = utf16ch & 0x003F; // 0000 0000 0011 1111
            ch8 = 0x80 + temp;  // 1000 0000
            fwrite(&ch8, sizeof ch8, 1, Fout);
        } else {
            if((utf16ch & 0x0780) != 0) { //0000 0111 1000 0000 скорее всего можно иначе и даже проще
                temp = utf16ch & 0x07C0; // & 0000 0111 1100 0000
                temp >>= 6;
                ch8 = 0xC0 + temp;  // 1100 0000
                fwrite(&ch8, sizeof ch8, 1, Fout);
                temp = utf16ch & 0x003F; // 0000 0000 0011 1111 ну либо так либо через две команды, но вроде не ругается
                ch8 = 0x80 + temp;
                fwrite(&ch8, sizeof ch8, 1, Fout);
            } else {
                if((utf16ch & 0xFF80) == 0) {
                    ch8 = utf16ch;
                    fwrite(&ch8, sizeof ch8, 1, Fout);
                } 
            }
        }
    utf16ch = 0;
    }
    if(feof(Fin) && utf16ch) fprintf(stderr, "Wrong(odd) numb of bytes, last is %X\n", utf16ch);
    fclose(Fin);
    fclose(Fout);
    return 0;
}

#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE* Fin = stdin;
    FILE* Fout = stdout;
    char ch8, bs = 0;
    unsigned short utf16ch = 0;  
    int flag = 0, numb = 0, bb = 0, np = 1, be = 1;
    if(argc > 1) {
        if((Fin = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "wrong input\n");
            return 1;
        }
        if(argc > 2)
            if((Fout = fopen(argv[2], "w")) == NULL) {
                fprintf(stderr, "wrong output\n");
                return 2;
            }
    }
    while((ch8 = fgetc(Fin)) != EOF) {
        numb++;
        if(utf16ch == 0) {
            utf16ch += ch8;
            if((ch8 & 0x80) != 0) {  // 1000 0000
                if((ch8 & 0xF0) == 0xE0) {  // & 1111 0000 = 1110 0000
                    for(int i = 1; i <= 2; i++) { // циклически проще, хотя можно  двумя IFами, но не работало на "битых"
                        utf16ch <<= 6;
                        ch8 = fgetc(Fin);
                        numb++;
                        if((ch8 & 0xC0) != 0x80) {  // & 1100 0000 = 1000 0000
                            bs = ch8;
                            bb = numb;
                        }
                        ch8 &= 0x3F;  // 0011 1111
                        utf16ch += ch8;  // сравнив прибавляем 
                    }
                    if((utf16ch == 0xFFFE) || (utf16ch == 0xFEFF)) { // проверка флаг = ютф16 и в итоге достаточно поменять местами байты в ютф16
                        flag = (utf16ch == 0xFFFE); //  1111 1111 1111 1110
                    }
                } else if((ch8 & 0xE0) == 0xC0) { // & 1110 0000 = 1100 0000
                    utf16ch <<= 6;
                    ch8 = fgetc(Fin);
                    numb++;
                    if((ch8 & 0xC0) != 0x80) {  // & 1100 0000 = 1000 0000
                        bs = ch8;
                        bb = numb;
                    }
                    ch8 &= 0x3F;  // ch = ch*00111111
                    utf16ch += ch8;
                    utf16ch &= 0x07FF;  
                } else {
                    bs = ch8;
                    bb = numb;
                }
            }
        }
        if(flag!=0){
            unsigned short sw = utf16ch & 0xFF00; // просто & на  1111 1111 0000 0000
            utf16ch <<=8; // свдигаем влево 
            sw >>=8; // сдвигаем вправо 
            utf16ch+=sw; // складываем и байты поменялись местами
        }
        if(bs && numb) {
            fprintf(stderr, "Broken %x in pos %d, skipping it\n", (unsigned char) bs, bb); // без unsigned char, выводит полный, хотя неясно ибо там и без того должна отсутствовать эта часть
            bs  = bb = 0;
        }
        else 
        {
            if (np){
              //  be = 0xFEFF;
                if (be == 0xFFFE){
                    fwrite(&be, sizeof utf16ch, 1, Fout);
                    fwrite(&utf16ch, sizeof utf16ch, 1, Fout);
                  //  printf("1");
                    np = 0;
                }
                if (be == 0xFEFF){
                    fwrite(&be, sizeof utf16ch, 1, Fout);
                    fwrite(&utf16ch, sizeof utf16ch, 1, Fout);
                  //  printf("2");
                    np = 0;
                }
            }
            else{ 
                fwrite(&utf16ch, sizeof utf16ch, 1, Fout);
            }
        }
        utf16ch = 0;
    }
    fclose(Fin);
    fclose(Fout);
    return 0;
}

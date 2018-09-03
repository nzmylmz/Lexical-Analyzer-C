//NAZIM YILMAZ 0516-0000-923 >> Assigment-1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//BeginningOfDefinateFunctions
void analyze(char kelime[],FILE *fp, char *keyList[]);
void sendWordbyWord(int i, char *keyList[]);
void keyPrint(char *keyList[]);
int fileSize(FILE *fp);
int * commentsAndStrings(char myChar[], char *keyList[], FILE *fp, int c, int row, char *lexical[], int i);
//EndOfDefinateFunctions

int main(int argc, char *argv[])
{
    FILE *fp;
    char word[1];
    char *keyList[17]={"{","}","int","move","to","add","sub","from","loop","times","out","\"",",","[","]","newline","."};

    printf("\n\t\t\tWELCOME TO LEXICAL ANALYZER\n");
    printf("\t\t\t---------------------------\n\n");

    printf("Please enter the file name : ");
    scanf("%s",argv[0]);

    if(strcmp(argv[0],"myscript"))
    {
        printf("\nIncorrect file name, the correct file name is 'myscript'\n");
    }
    else
    {
        strcat(argv[0],".ba");
        fp = fopen(argv[0],"r");
        if( fp==NULL )
        {
            printf("\nLoading Error : '%s'\n",argv[0]);
            printf("------------------------\n\n");
        }
        else
        {
            printf("\nLoaded File : '%s'\n",argv[0]);
            printf("------------------------\n\n");
        }

    keyPrint(keyList);
    analyze(word,fp,keyList);
    free(fp);
    free(keyList);
    fclose(fp);
    }
return 0;
}
void analyze(char myChar[],FILE *fp, char *keyList[])
{
    int *info;
    int error=0;
    int c=0,row=1,i=0,sParan=0;
    char *lexical[(fileSize(fp))];
    FILE *lexeme;
    lexeme = fopen("result.txt","w");

    while (c != EOF)
    {
        c=fscanf(fp,"%c",myChar);
        if (c != EOF)
        {
            if((int)myChar[0]==10)
            {
                row++;
            }
            while (c != EOF)
            {
                if((myChar[0]==*keyList[0])||(myChar[0]==*keyList[11]))
                {
                    info=commentsAndStrings(myChar,keyList,fp,c,row,lexical,i);
                    c=fscanf(fp,"%c",myChar);
                    row=*(info+0);
                    i=*(info+1);
                    if(row==0)
                    {
                        error=1;
                        break;
                    }
                }else break;
                if((int)myChar[0]==10)
                {
                    row++;
                }
            }
            if(c!=EOF)
            {
                if(((int)myChar[0]>=97)&&((int)myChar[0]<=122)||//a..z
                   ((int)myChar[0]>=65)&&((int)myChar[0]<=90)||//A..Z
                   ((int)myChar[0]>=48)&&((int)myChar[0]<=57)||//0..9
                   ((int)myChar[0]==32)||((int)myChar[0]==10)||// space and new line
                   ((int)myChar[0]==91)||((int)myChar[0]==93)||//[ ]
                   ((int)myChar[0]==46)||((int)myChar[0]==44)||((int)myChar[0]==45)) // . ,
                  {
                      if((int)myChar[0]==46) //.
                      { lexical[i++]=10;
                        lexical[i++]=myChar[0];
                        lexical[i++]=10;
                      }
                      else if((int)myChar[0]==44)//,
                      {
                          i--;
                          if(!(((int)lexical[i]>=48)&&((int)lexical[i]<=57)))//0..9)
                            {
                                i++;
                                lexical[i++]=10;
                                lexical[i++]=myChar[0];
                                lexical[i++]=10;
                            }
                            else
                            {
                                i++;
                                lexical[i++]=myChar[0];
                            }
                      }else if (((int)myChar[0]==91)||((int)myChar[0]==93))//[ ]
                      {
                          if((int)myChar[0]==91)
                            sParan=row;
                          if(((int)myChar[0]==93)&&(sParan!=0))//acildiysa kapanir
                          {
                              sParan=0;
                          }else if(((int)myChar[0]==93)&&(sParan==0))//acilmadan kapanir
                          {
                              printf("\nERROR : Character %c turned off on line %d was not opened \n",*keyList[14],row);
                              error=1;
                              break;
                          }
                        lexical[i++]=10;
                        lexical[i++]=myChar[0];
                        lexical[i++]=10;
                      }
                      else
                      {
                        lexical[i++]=myChar[0];
                      }
                   }
                else if(myChar[0]==*keyList[1])
                    {
                        printf("\nERROR : Character %c turned off on line %d was not opened \n",*keyList[1],row);
                        error=1;
                        break;
                    }
                else if(myChar[0]==*keyList[14])
                    {
                        printf("\nERROR : Character %c turned off on line %d was not opened \n",*keyList[14],row);
                        error=1;
                        break;
                    }
                else
                    {
                        printf("\nERROR : Line %d has an unidentified character %c (ASCII %d)\n",row,myChar[0],myChar[0]);
                        error=1;
                        break;
                    }
            }
        }
    }
    if(sParan!=0)
    {
         printf("\nERROR : Character %c turned opened on line %d was not closed\n",*keyList[13],sParan);
         error=1;
    }

    if(error==0)
    { int x=0;
        for(x=0;x<i;x++)
            fputc(lexical[x],lexeme);
        fclose(lexeme);
        sendWordbyWord(i,keyList);
    }
    free(lexical);
    fclose(fp);
    fflush(fp);
    getchar();
}
void sendWordbyWord(int i, char *keyList[])
{
    int c=0,g=0,counter=0,control=0,keyWord=0,intConst=0,str=0;
    char word[101]={10};
    int keyListNumber[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    FILE *token;
    token= fopen("token.txt","w");
    char *descrip;
    FILE *lexeme;
    lexeme= fopen("result.txt","r");

    fputs("Lexical  Token\n-------  ------\n",token);

    while (c != EOF)
    {
        c=fscanf(lexeme,"%s",word);

        if ((c != EOF))
        {
            for(g=0;g<17;g++)
            {
                if(!strcmp(word,keyList[g]))
                {
                    descrip="Keyword";
                    keyWord+=1;
                    if(!strcmp(word,keyList[16]))
                        descrip="EndOfLine";
                    else if(!strcmp(word,keyList[13]))
                        descrip="OpenParenthesis";
                    else if(!strcmp(word,keyList[14]))
                        descrip="CloseParenthesis";
                    else if(!strcmp(word,keyList[3]))
                        descrip="Assignment Operator";
                    else if(!strcmp(word,keyList[5]))
                        descrip="Adding Operator";
                    else if(!strcmp(word,keyList[6]))
                        descrip="Subtract Operator";

                    keyListNumber[g]+=1;
                    fprintf(token,"%s\t%s\n",word,descrip);
                    break;
                }
                else if(g==16)
                {
                    descrip="Identifier";
                    if(isdigit(word[0])!=0)
                        {
                            intConst+=1;
                            descrip="Int_Const";
                            fprintf(token,"%s\t%s\n",word,descrip);
                            break;
                        }
                    else if((word[0]=='"')||(control==1))
                    {
                        printf("\n");
                        descrip="String_Const";
                        str+=1;
                        while (c != EOF)
                        {
                            if (c != EOF)
                            {
                                fputs(word,token);
                                fputc('\0',token);
                                control=1;
                                if (word[strlen(word)-1]=='"')
                                    {
                                        control=0;
                                        break;
                                    }
                            }
                            c=fscanf(lexeme,"%s",word);
                        }
                        fprintf(token,"\t%s\n",descrip);
                        break;
                    }
                    counter++;
                    fprintf(token,"%s\t%s\n",word,descrip);
                }
            }
        }
    }
    printf("KeyWords&Const\t\tNumber of Total\n--------------\t\t---------------\n");
    int totalKeyword=0;
    for(g=0;g<17;g++)
       {
            if(keyListNumber[g]!=0)
                printf("%s\t\t>>\t\t%d\n",keyList[g],keyListNumber[g]);
            totalKeyword+=keyListNumber[g];
       }
       printf("int_consts\t>>\t\t%d\n",intConst);
       printf("string_consts\t>>\t\t%d\n",str);
       printf("variables\t>>\t\t%d\n",counter);

       printf("\n\nTotal Number of Lexemes Found\n");
       printf("-----------------------------\n");
       fputs(("\n\nTotal Number of Lexemes Found\n-----------------------------\n"),token);
       printf("Identifiers : %d\n",counter);
       fprintf(token,"Identifiers : %d\n",counter);
       printf("Parenthesis : %d\n",keyListNumber[13]+keyListNumber[14]);
       fprintf(token,"Parenthesis : %d\n",keyListNumber[13]+keyListNumber[14]);
       printf("Keywords    : %d\n",totalKeyword);
       fprintf(token,"Keywords    : %d\n",totalKeyword);
       printf("End Of Line Characters : %d\n",keyListNumber[16]);
       fprintf(token,"End Of Line Characters : %d\n",keyListNumber[16]);
    fclose(lexeme);
    remove("result.txt");
    fclose(token);
}
void keyPrint(char *keyList[])
{
    int i=0;
        printf("Specified\nCharacters &\nKeyword List\n------------\n");
        for(i=0;i<17;i++)
        {
            printf("%d - %s\n",i+1,keyList[i]);
        }
}
int fileSize(FILE *fp)
{
    int length;
      fseek(fp,0,SEEK_END);
      length=ftell(fp);
      printf("\n------------------------------\n");
      printf("The file's length is %d Byte\n",length);
      printf("------------------------------\n");
      fseek(fp,0,SEEK_SET);
    return length;
}
int * commentsAndStrings(char myChar[], char *keyList[], FILE *fp, int c, int row, char *lexical[],int i)
{
    static int info[2];
    int x,oldRow=row;

    if(myChar[0]==*keyList[0])//if x= '{' then
    {
        x=1;
    }
    else if(myChar[0]==*keyList[11]) //if x= ' " ' then
    {
        x=11;
    }
    while (c != EOF)
    {
        if(x==11)
        {
            lexical[i++]=myChar[0];
        }
        c = fscanf(fp,"%c",myChar);
        if (c != EOF)
        {
            if((int)myChar[0]==10)
            {
                row++;
            }
            if(myChar[0]==*keyList[x])//[strlen(kelime)-1]
            {
                if(x==11)
                {

                     lexical[i++]=myChar[0];
                }
                break;
            }
        }
        else if(c==EOF)
        {
            printf("\nERROR : Character %c opened on line %d was not turned off\n",*keyList[x],oldRow);
            row=0;
        }
    }
    info[0]=row;
    info[1]=i;
    return info;
}

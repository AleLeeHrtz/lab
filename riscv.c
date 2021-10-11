#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
    int reg_amount = 32;
    reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
    for(int i = 0; i < 32; i++)
        reg[i] = i;
}

int regRet(char* token){
    char res[500];

    int i = 1;
    char temp = token[0];
    while (temp != ')' || temp == ' ' || temp == '\0'){
        temp = token[i];
        res[i - 1] = temp;
        i++;
    }

    int result = atoi(res);
    return result;
}

/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
bool interpret(char* instr){

    char* token = strtok(instr, " ");

    switch (token[0]) {

        //LW case
        case 'L':

            //Setting apart rd1
            token = strtok(NULL, " ");
            char* rdst = token;

            //Setting apart rs2
            token = strtok(NULL, " ");
            char* rsrc = token;

            char* brkdown;


            //Obtaining immediate from raw string
            brkdown = strtok(rsrc, "(");
            int32_t consval = atoi(brkdown);
            printf("Consval = %x\n", consval);


            //Obtaining register from raw string
            brkdown = strtok(NULL, "(");
            int source = regRet(brkdown);
            printf("Found in reg %x\n", reg[source]);

            //Obtaining register from rd1 string
            int dest = regRet(rdst);

            //If address result is not appropriate, fails to load from it and returns false.
            if ((reg[source] + consval) > 198 || (reg[source] + consval) < 0 || (reg[source] + consval) % 2 != 0 || dest == 0){
                printf("Address %X is not valid. Ensure it is a multiple of 2, it is not register X0 and it is between 0 and 18C.\n", (reg[source] + consval));
                return false;
            }

            //Address is being read from.
            int32_t temp = (reg[source] + consval);
            printf("Address (0x%lX)\n", temp);
            reg[dest] = read_address( temp, "mem.txt");
            printf("Reg %i = %X\n", dest, reg[dest]);


            break;



            //SW case
        case 'S':
            //Setting apart rd1
            token = strtok(NULL, " ");
            char* rsrc1 = token;

            //Setting apart rs2
            token = strtok(NULL, " ");
            char* rdst1 = token;

            char* brkdown1;

            //Obtaining immediate from raw string
            brkdown1 = strtok(rdst1, "(");
            int32_t consval1 = atoi(brkdown1);
            printf("Consval = %X\n", consval1);

            //Obtaining register from raw string
            brkdown1 = strtok(NULL, "(");
            int dest1 = regRet(brkdown1);
            printf("First address is %X\n", dest1);

            //Obtaining register from rd1 string
            int source1 = regRet(rsrc1);
            printf("Found in reg %X\n", reg[source1]);

            //If address result is not appropriate, fails to load from it and returns false.
            if ((reg[dest1] + consval1) > 198 || (reg[dest1] + consval1) < 0 || (reg[dest1] + consval1) % 2 != 0){
                printf("Address %X is not valid. Ensure it is a multiple of 2 and it is between 0 and 0C6.\n", (reg[source1] + consval));
                return false;
            }

            //Address is being writen at.
            int32_t tempadrs = (reg[dest1] + consval1);
            printf("Address (0x%lX)\n", tempadrs);
            write_address(reg[source1], tempadrs, "mem.txt");

            break;
        case 'A':

            printf("TOken[3]: %c", token[3]);
            if(token[3] == 0 ){
                //ADD case

                //Obtaining destination register
                printf("Inside if");
                token = strtok(NULL, " ");
                printf("First token obtained");
                char* regdst = token;
                printf("Char generated");
                int d1 = regRet(regdst);


                //Obtaining rs1
                token = strtok(NULL, " ");
                char* rsource1 = token;
                int rs1 = regRet(rsource1);

                //Obtaining rs2
                token = strtok(NULL, " ");
                char* rsource2 = token;
                int rs2 = regRet(rsource2);

                //Adding values
                printf("Previous value of reg %i was %X\n", d1, reg[d1]);
                printf("Values to add: 0x%lX %lX\n", reg[rs1], reg[rs2]);
                reg[d1] = reg[rs1] + reg[rs2];
                printf("Updated value of reg %i is now %X\n", d1, reg[d1]);



            }
            else{
                //ADDI case

                //Obtaining register destination
                token = strtok(NULL, " ");
                char* rgdst = strtok(NULL, " ");

                //Obtaining register source
                token = strtok(NULL, " ");
                char* rs = strtok(NULL, " ");

                //Obtaining immediate value
                token = strtok(NULL, " ");
                char* immVal = strtok(NULL, " ");
                //Debuged

                //Obtaining register destination
                int d = regRet(rgdst);

                //Obtaining register source
                int s = regRet(rs);

                //Obtaining immediate
                int32_t imm = atoi(immVal);

                reg[d] = reg[s] + imm;

            }
            break;
        default:
            printf("It is not a valid instruction, try again.\n");
            return false;
            break;

    }

    return true;
}


/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
    int32_t data_to_write = 0xFFF; // equal to 4095
    int32_t address = 0x98; // equal to 152
    char* mem_file = "mem.txt";

    // Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
    int32_t write = write_address(data_to_write, address, mem_file);
    if(write == (int32_t) NULL)
        printf("ERROR: Unsucessful write to address %0X\n", 0x40);
    int32_t read = read_address(address, mem_file);

    printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}

/**
 * Your code goes in the main
 *
 */
int main(){
    // Do not write any code between init_regs
    init_regs(); // DO NOT REMOVE THIS LINE

    // Below is a sample program to a write-read. Overwrite this with your own code.
    //write_read_demo();

    char samp[500];

    while(true){
        printf("Enter a instruction separated by spaces ONLY.\n");
        fgets(samp, 500 , stdin);

        bool result = interpret(samp);
        fflush(stdin);
        if(result){
            printf("True\n");
        }
        else{
            printf("False\n");
        }
    }


    return 0;
}

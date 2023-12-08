
#include<stdio.h>
#include<stdint.h>

// Get 64 bytes of data from any 2 boards
uint8_t d1[] = {0x90,0xF2,0xEA,0x36,0xD1,0xB7,0x3A,0x10,0xC9,0xCE,0x98,0xC4,0xC9,0x9E,0x88,0xEC,0xC9,0xA3,0xA2,0x3A,0x9C,0xF1,0x54,0x39,0xF5,0x7D,0xB1,0xB5,0x89,0x79,0x90,0xE8,0xA8,0x6B,0xA3,0xAC,0xCC,0x45,0xF2,0xF4,0x54,0xD5,0xF9,0x7F,0xDC,0x8A,0xE2,0x6A,0xAB,0xC8,0xF8,0x76,0x6D,0xD1,0x89,0x44,0x5E,0xF9,0xDD,0x1F,0x76,0x8A,0x8B,0x2A};
uint8_t d2[] = {0xA5,0xAA,0xEE,0xB3,0x27,0xD6,0xE4,0x99,0x64,0x95,0x13,0xCF,0xB6,0xA0,0xE7,0x9F,0xAF,0xA4,0xFB,0xE2,0x62,0xF5,0x64,0xF5,0x31,0x17,0x55,0x10,0x62,0x32,0x68,0x9F,0x81,0xEE,0xAF,0xB2,0x45,0x15,0x45,0xD4,0x70,0x47,0x00,0x58,0x4F,0xE7,0xE1,0xB9,0x7F,0x8A,0x69,0x60,0x40,0xAA,0x51,0x00,0xF8,0x46,0x1C,0x80,0xEA,0xC6,0x3C,0x02};

// Get 64 bytes of Mask from any 2 boards that indicates stable bits
uint8_t M1[] = {183, 175, 183, 123, 247, 179, 243, 247, 95, 205, 53, 156, 215, 127, 125, 59, 127, 255, 247, 191, 62, 251, 215, 191, 239, 223, 255, 237, 159, 127, 215, 253, 223, 253, 254, 255, 75, 255, 254, 190, 255, 252, 167, 239, 239, 249, 159, 219, 189, 79, 190, 221, 255, 254, 203, 237, 95, 157, 255, 207, 246, 223, 181, 119};
uint8_t M2[] = {253, 223, 255, 255, 183, 235, 255, 113, 223, 255, 251, 123, 185, 223, 247, 223, 191, 255, 107, 247, 221, 255, 255, 251, 255, 237, 163, 255, 126, 247, 118, 159, 219, 253, 253, 255, 239, 127, 191, 253, 255, 251, 255, 255, 223, 255, 255, 255, 189, 255, 251, 213, 127, 95, 255, 253, 207, 191, 255, 253, 221, 255, 31, 255};

int main()
{
    uint16_t num_valid_bits_board1 = 0, num_valid_bits_board2 = 0; 
    uint16_t num_valid_0_board1 = 0,num_valid_1_board1 = 0; 
    uint16_t num_valid_0_board2 = 0,num_valid_1_board2 = 0;
    uint32_t SRAM_128bit_PUF_BOARD1[4]={0};
    uint8_t num_valid_1s_PUF1 = 0; 
    uint32_t SRAM_128bit_PUF_BOARD2[4]={0};
    uint8_t num_valid_1s_PUF2 = 0; 
    uint8_t interchip_hamming_distance=0;
    
    printf("First board stable SRAM cell values:\n");
    for(int i=0;i<64;i++)
    {
        for(int j=0;j<8;j++)
        {
            uint8_t bit_val;
            if(M1[i] & (1 << j))
            {
                bit_val = (d1[i] & (1 << j)) >> j;
                bit_val == 0?num_valid_0_board1++:num_valid_1_board1++;
                printf("%x",bit_val);
                if(num_valid_bits_board1 < 128)
                {
                    SRAM_128bit_PUF_BOARD1[num_valid_bits_board1/32] |=  ((uint32_t)bit_val) << (31-((num_valid_bits_board1)%32));
                    if(bit_val)
                        num_valid_1s_PUF1++;
                }
                
                num_valid_bits_board1++;
            }
        }
    }
    printf("\nBoard 1 PUF:\n");
    for(int i=0;i<128;i++)
        printf("%d",((SRAM_128bit_PUF_BOARD1[i/32] >> (31-((i)%32)))&0x1));
    
    printf("\nSecond board stable SRAM cell values:\n");
    
    for(int i=0;i<64;i++)
    {
        for(int j=0;j<8;j++)
        {
            uint8_t bit_val;
            if(M2[i] & (1 << j))
            {
                bit_val = (d2[i] & (1 << j)) >> j;
                bit_val == 0?num_valid_0_board2++:num_valid_1_board2++;
                printf("%x",bit_val);
                if(num_valid_bits_board2 < 128)
                {
                    SRAM_128bit_PUF_BOARD2[num_valid_bits_board2/32] |=  ((uint32_t)bit_val) << (31-((num_valid_bits_board2)%32));
                    if(bit_val)
                        num_valid_1s_PUF2++;
                }
                
                num_valid_bits_board2++;
            }
        }
    }
    printf("\nBoard 2 PUF:\n");
    for(int i=0;i<128;i++)
        printf("%d",((SRAM_128bit_PUF_BOARD2[i/32] >> (31-((i)%32)))&0x1));
    printf("\n");
    
    
    for(int i=0;i<128;i++)
        if(((SRAM_128bit_PUF_BOARD2[i/32] >> (31-((i)%32)))&0x1)!=((SRAM_128bit_PUF_BOARD1[i/32] >> (31-((i)%32)))&0x1))
            interchip_hamming_distance++;
    printf("Number of stable bits in first board:%d\n", num_valid_bits_board1);
    printf("Number of stable 0 bits in first board:%d\n", num_valid_0_board1);
    printf("Number of stable 1 bits in first board:%d\n", num_valid_1_board1);
    printf("Number of stable bits in second board:%d\n", num_valid_bits_board2);
    printf("Number of stable 0 bits in second board:%d\n", num_valid_0_board2);
    printf("Number of stable 1 bits in second board:%d\n", num_valid_1_board2);
    printf("Number of stable 1 bits in PUF-1:%d\n", num_valid_1s_PUF1);
    printf("Number of stable 1 bits in PUF-2:%d\n", num_valid_1s_PUF2);
    printf("Interchip Hamming distance:%.02f percent\n", interchip_hamming_distance/1.28f);
    printf("Uniformity of PUF-1:%.02f percent\n", num_valid_1s_PUF1/1.28f);
    printf("Uniformity of PUF-2:%.02f percent\n", num_valid_1s_PUF2/1.28f);
    return 0;
}

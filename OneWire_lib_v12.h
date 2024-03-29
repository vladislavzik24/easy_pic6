// ���������� ��� ������ � ����������� 1-Wire � �������� ����������� BS18B20
// v1.2.0_2012-07-28
// ���������� ���������: ���������� ����� EasyPIC 6
// �����������: ������� Delay_us(), Delay_ms() ����� microC PRO for PIC
// �����: ���� ������, �.�. ����� (Ratgor)
// �����������: �.�. ����� (Ratgor)



/*
// ��������� ����������� ������� � �����
#define DS_TRIS TRISA    // 8-������ ������� ����������� ����� �
#define DS_PORT PORTA    // 8-������ ������� �������� ����� �
#define DS 5             // ����� ������������ ����� �����
*/



// ��������� �������:

// ������� �������
void          OneWire_Byte_Tx (unsigned char);    // �������� ������ �����
unsigned char OneWire_Byte_Rx (void);             // ����� ������ �����
unsigned char OneWire_Reset   (void);             // ����� ��������� �� �����
unsigned char OneWire_Match_ROM_Tx                // �������� ������ (������� ����) �� ROM(ASCII)
(unsigned char *, unsigned char *, unsigned char);

// ������ ��������������� DS18B20
unsigned char DS_ConvT          (void);           // �������������� ����������� ��� ���� �������� �� �����
unsigned char DS_Read           (void);           // ������ ����������� � ������ ASCII
unsigned char DS_Read_ROM       (void);           // ������ ����������� ROM �������������� � ������ ASCII
unsigned char DS_Match_ROM      (unsigned char*); // ������ ����������� �� ROM(ASCII) � ������ ASCII
double        DS_ReadTemp_byROM (unsigned char*); // ������ ����������� � ����� � ��������� ������


// ������� ������
unsigned char OneWire_Slave_Presence  (void);                  // ������������� ���������� �� ����� (����������� �������)
unsigned char OneWire_Slave_Byte_Rx   (void);                  // ������ ������ ����� (����������� �������)
void          OneWire_Slave_Byte_Tx   (unsigned char);         // �������� ������ ����� (����������� �������)
void          OneWire_Slave_Read_ROM  (unsigned char *ROM);    // ������� ������� ROM-����� (����������� �������)
unsigned char OneWire_Slave_Match_ROM (unsigned char *ROM);    // ������� ������� ROM-����� (����������� �������)



// ������� �������� ������ ����� �� ���������� 1-Wire
void OneWire_Byte_Tx (unsigned char whu){

    // ����������-������� ���������� ���������� ���
    unsigned char i;

    // ���� �������� �������� �����, ������� � �������� ����
    for(i=0;i<8;i++){

        // �������� ������� ���� �������������� � ���� ����� (��)
        // (��� ������� ������� ������������� 60-120 ��� + 1 ��� ����� ���� ��)
        // ������ �� ���������� ���������� ����� (1-15 ���)
        // ������ ��� ������������ ���������� ����� ����� ��������� �����
        // ����� ������ ���� �����. �������� ���� = 0 ���������� �������
        // �������� ��������� ����� � ������ ���������, ��� �������� ���� = 1

        // ���� ������������ ��� = 1
        if(whu &(1<<i)){
            // ����������� ����� �� �����
            DS_TRIS &= ~(1<<DS);
            // ��������� ����� � ����� ������� �� ����� ����������� ����
            DS_PORT &= ~(1<<DS);
            // ������ �� ����� 2 ���(��� �������� �������)
            Delay_us(2);
            // ��������� �����, �������� �� �� ����
            DS_TRIS |= (1<<DS);
            // ����� ��� ���������� ���� �����
            Delay_us(93);
        }
        // ���� ������������ ��� = 0
        else{
            // ����������� ����� �� �����
            DS_TRIS &= ~(1<<DS);
            // ��������� ����� � ����� ������� �� ����� ����������� ����
            DS_PORT &= ~(1<<DS);
            // ��� �������� ���� ����� �������� ����� � ����� ������
            Delay_us(83);
            DS_TRIS |= (1<<DS);
            Delay_us(12);
        }
    }
    // ����� ������� ����������� ����� ����� ��������� ��������� �����
    // ��� �� ��� �� ������ � �������� ���������, �������� ����� �����
    Delay_us(500);

}



// ������� ������ ������ �����
unsigned char OneWire_Byte_Rx (void){

    // ����������: �������� ���� � ������� ���������� �������� ���
    unsigned char temp=0, i;

    // ���� ��������� ������ �����, ������� � �������� ����
    for(i=0;i<8;i++){

        // �������� ������� ���� �������������� � ���� ����� (��)
        // (��� ������� ������� ������������� 60-120 ��� + 1 ��� ����� ���� ��)
        // ������ �� ���������� ���������� ����� (1-15 ���)
        // ������ ��� ������������ ���������� ����� ����� ��������� �����
        // ����� ������ ���� �����. �������� ���� = 0 ������������ ����������
        // ����� ������� �����������, �������� ���� = 1 - ����������� ���������

        // ��������� �� ��� ������:
        DS_TRIS &= ~(1<<DS);
        DS_PORT &= ~(1<<DS);
        // ����������� ��������� ������ �� (����� 1 ���)
        Delay_us(1);
        // � ��������� �����.
        DS_TRIS |= (1<<DS);
        // ���� ������ ������ �������� ���������� (����� 13 ���)
        Delay_us(13);
        // � ������ ������.

        // ���������� ��������� ����� ����� ������������� ������������� ����.
        // �������� ���������� � �������� ����� � �������� ����.

        // ���� = 1, �� ������������� � �������� ������� ������ ������� ����� 1
        if(DS_PORT & (1<<DS)){
            temp |= 128;
        }
        // ���� = 0,�� ��� � ��������� � ������� ���� 0
        else{
            //temp >>= 1;
        }
        // ���� ��������� �� ��������� ���, �� �������� ����� �������� ������� ������
        if(i<7)
            temp>>=1;
        // �������� ������������ ������ ����� � ���������
        Delay_us(100);
    }
    // �������� ������������ � �������� ����������, ����������� �����
    Delay_us(500);

    // �� ��������� ������ 8 ���, ���������� �������������� ����
    return temp;
}



// ������� ������������ �������� ������ �� �����
unsigned char OneWire_Reset (void){

    // ������ ������ � ����� ����������� ������������ �������� ������ �����
    // ��� ������ ������� ���������� �������� ����� �� ����� 480 ���
    // ����� ������ ����� ������� ���������� ���������� ���� ����������� �� �����
    // ��� ����������� ����������� ����� ���������� ����� ������� �����������,
    // ����� �������� ������� ���������� (����������� ��������� - �� 60-240 ���)

    // ��� ������ ��������� ����� � ����� �� ����� �� ����� 480 ���
    DS_TRIS &= ~(1<<DS);
    DS_PORT &= ~(1<<DS);
    Delay_us(500);
    DS_TRIS |= (1<<DS);
    // ����� ������, ���� ���� ���������� �� �����, ��������� �����
    Delay_us(40);
    // ���� ������� ����� = 0, �� ���������� ����, � ��������� ������ ��� ���.
    // ���� ���������� ��� �� �����, ���������� 1
    if(DS_PORT&(1<<DS)){
        Delay_us(500);
        return 1;
    }
    // ��� ������� ���������� �� �����, ���������� 0
    Delay_us(500);
    return 0;
}



// ������� ��������� ��������� �������, ��������������� ����� � ASCII �������,
// �������� �� � ������ � ���������� ��������� �� ���� ������
unsigned char *DS_Read(void){

    // ���������� ��� �������� � �������� ���� �������� ���������� �����������
    unsigned char tempH, tempL;
    // ���������� ��� ASCII ��������, ��������������� ���������� �����������
    // �������� 0x30 ������������� ������ ���� "0,1,2,3..." � ASCII �������
    unsigned char znak=0, doDot=0, posleDot=0, d100=0x30, d10=0x30, d1=0x30, p1000=0x30, p100=0x30, p10=0x30, p1=0x30;
    // ���������� ��� �������������� ������� ����� �������� ���������� �����������
    int pDec=0;
    // ������ � ������� �������� ������ ��� ������ �� ���������
    static unsigned char DispTemp[10];

    // �������:
    // ����� ������������ (����� ���� ��������� �� �����)
    // ������� ROM (����� ������������) (� ��� ������ ���� ���������� �� �����)
    // ������ �������������� ����������� (���� ����������� �� �����)
    if(OneWire_Reset()){
        DispTemp[0] = 'N';
        DispTemp[1] = 'o';
        DispTemp[2] = ' ';
        DispTemp[3] = 'S';
        DispTemp[4] = 'e';
        DispTemp[5] = 'n';
        DispTemp[6] = 's';
        DispTemp[7] = 'o';
        DispTemp[8] = 'r';
        DispTemp[9] = '\0';
        return &DispTemp;
    }
    OneWire_Byte_Tx (0xCC);
    OneWire_Byte_Tx (0x44);

    // ���� ��������� �����. ����� �������������� ����������� �� 100 �� 750 ��.
    Delay_ms(750);

    // �������:
    // ����� ������������ (����� ���� ��������� �� �����). ���� ��������� ���, ������ ������.
    // ������� ROM (����� ������������) (� ��� ������ ���� ���������� �� �����)
    // ������ ������ ������ � ����������� ����������� (���� ����������� �� �����)
    if(OneWire_Reset()){
        DispTemp[0] = 'N';
        DispTemp[1] = 'o';
        DispTemp[2] = ' ';
        DispTemp[3] = 'S';
        DispTemp[4] = 'e';
        DispTemp[5] = 'n';
        DispTemp[6] = 's';
        DispTemp[7] = 'o';
        DispTemp[8] = 'r';
        DispTemp[9] = '\0';
        return &DispTemp;
    }
    OneWire_Byte_Tx (0xCC);
    OneWire_Byte_Tx (0xBE);

    // ������ ���� ���� �������� ���������� �����������
    // ���� ���������� ����������� ��������� � ������� ����� (������� 5 ���)
    // ������������� �������� ����������� ��������� � ������� � ������� ������
    // ������� �������� ����������� ��������� � ������� ����� (������� 4 ����)

    // ������� ���� �������� ���������� �����������
    tempL = OneWire_Byte_Rx ();
    // ������� �������� ���������� �����������
    tempH = OneWire_Byte_Rx ();

    // ���������� ���� ����������� (5 ������� ��� � ������� �����)
    // ���� = 1 �� ������������� �����������
    if(tempH & 128)
        znak = 0x2D;  // ��� ����� "-"  � ASCII
    // ���� 0 - �� �������������
    else
        znak = 0x2B;   // ��� ����� "+"  � ASCII

    // �������� ����� � ������� ����� �������� ���������� �����������
    // � ��������� ����������
    posleDot = (tempL & 0x0F);  // C�������� ������� ����� � ����������
    doDot |= (tempL>>4);        // ��������� ������� 4 ���� ����� �����
    doDot |= ((tempH&0x07)<<4); // ��������� ������� 4 ���� ����� �����

    // ��������� ���������� �������� �� ������� ����� �������� � ������
    // � ��������� � ��������������� ASCII �������

    // ���������� ������� ����� �������� (����� � ASCII)
    while(doDot>=100){
        doDot-=100;
        d100++;
    }
    // ���������� ������� �������� �������� (����� � ASCII)
    while(doDot>=10){
        doDot-=10;
        d10++;
    }
    // ���������� ������� ������ �������� (����� � ASCII)
    while(doDot>=1){
        doDot-=1;
        d1++;
    }

    // ��������������� ������� ����� ����� ������������� ����������
    // ���������� ��� int ������ double
    // ������� ��� ������ �� 1000, ����� ����� ��������� ����� (0,0625) ����� �����

    // ������ ������ ������� ����� ������������� 0,5
    if(posleDot&8)
        pDec = 5000;

    // ������ ������ ������� ����� ������������� 0,25
    if(posleDot&4)
        pDec += 2500;

    // ������ ������ ������� ����� ������������� 0,125
    if(posleDot&2)
        pDec += 1250;

    // ������� ������ ������� ������������� 0,0625
    if(posleDot&1)
        pDec += 625;

    // ��������� ���� ����� ������ ������ (����� � ASCII)

    // ������� ���� �������
    while(pDec>=1000){
        pDec -= 1000;
        p1000++;
    }
    // ����� ���� �������
    while(pDec>=100){
        pDec -= 100;
        p100++;
    }
    // �������� ���� �������
    while(pDec>=10){
        pDec -= 10;
        p10++;
    }
    // �������������� ���� �������
    while(pDec>=1){
        pDec -= 1;
        p1++;
    }

    // ��� ��� ��� ����������� ���������� �������� ���������� �����������
    // �� ���������� �������������� �������� � �������� � ����� 0x30,
    // �� �� ����� �������� �������� ���� � ASCII ����.

    // ������� �� � ������
    DispTemp[0] = znak;    // ����
    DispTemp[1] = d100;    // ����� �����, �����
    DispTemp[2] = d10;     // ����� �����, �������
    DispTemp[3] = d1;      // ����� �����, �������
    DispTemp[4] = '.';     // ���������� �����
    DispTemp[5] = p1000;   // ������� �����, �������
    DispTemp[6] = p100;    // ������� �����, �����
    DispTemp[7] = p10;     // ������� �����, ������
    DispTemp[8] = p1;      // ������� �����, ������� �����
    DispTemp[9] = '\0';    // ����� ���������� �������

    // ���������� ��������� �� ������
    return &DispTemp;
}




// ������� ���� ������� �������������� ����������� ���� ����������� �� �����
unsigned char *DS_ConvT(void){

    // ������ � ������� �������� ������ ��� ������ �� ���������
    static unsigned char DispTemp[10];

    // �������:
    // ����� ������������ (����� ���� ��������� �� �����)
    // ������� ROM (����� ������������) (� ��� ������ ���� ���������� �� �����)
    // ������ �������������� ����������� (���� ����������� �� �����)
    if(OneWire_Reset()){
        DispTemp[0] = 'N';
        DispTemp[1] = 'o';
        DispTemp[2] = ' ';
        DispTemp[3] = 'S';
        DispTemp[4] = 'e';
        DispTemp[5] = 'n';
        DispTemp[6] = 's';
        DispTemp[7] = 'o';
        DispTemp[8] = 'r';
        DispTemp[9] = '\0';
        return &DispTemp;
    }
    OneWire_Byte_Tx (0xCC);
    OneWire_Byte_Tx (0x44);

    // ���� ��������� �����. ����� �������������� ����������� �� 100 �� 750 ��.
    Delay_ms(750);

    DispTemp[0] = 'C';
    DispTemp[1] = 'o';
    DispTemp[2] = 'n';
    DispTemp[3] = 'v';
    DispTemp[4] = 'R';
    DispTemp[5] = 'e';
    DispTemp[6] = 'a';
    DispTemp[7] = 'd';
    DispTemp[8] = 'y';
    DispTemp[9] = '\0';
        
        return &DispTemp;
}



// ������� ��������� ���������� ROM ������������� ������� (8 ����),
// ��������������� ����� � ASCII �������,
// �������� �� � ������ � ���������� ��������� �� ���� ������
unsigned char *DS_Read_ROM(void){

    // ����������-������� ��� ������� ���� � ROM-������
    unsigned char ROM_Counter=8;
    // ������ ��� �������� �������� �� ROM-������
    unsigned char ROM_Buffer;
    // ������ � ������� �������� ������ ��� ������ �� ���������
    static unsigned char DispTemp[17];

    // �������:

    // ����� ������������ (����� ���� ��������� �� �����)
    if(OneWire_Reset()){
        DispTemp[0] = 'N';
        DispTemp[1] = 'o';
        DispTemp[2] = ' ';
        DispTemp[3] = 'D';
        DispTemp[4] = 'e';
        DispTemp[5] = 'v';
        DispTemp[6] = 'i';
        DispTemp[7] = 'c';
        DispTemp[8] = 'e';
        DispTemp[9] = '\0';
        return &DispTemp;
    }

    // ������ �������� ROM (��� ���� ��������� �� �����)
    OneWire_Byte_Tx (0x33);

    // �������� ����������� ROM (8 ����) � ������ ASCII ��������
    while(ROM_Counter--){
        ROM_Buffer = OneWire_Byte_Rx();
        if((ROM_Buffer/16)<0x0A)
            DispTemp[ROM_Counter*2] = ROM_Buffer/16 + 0x30;
        else
            DispTemp[ROM_Counter*2] = ROM_Buffer/16 + 0x37;
        if((ROM_Buffer%16)<0x0A)
            DispTemp[ROM_Counter*2+1] = ROM_Buffer%16 + 0x30;
        else
            DispTemp[ROM_Counter*2+1] = ROM_Buffer%16 + 0x37;
    }
    DispTemp[16] = '\0';

    // ���������� ��������� �� ������
    return &DispTemp;
}



// ������� ��������� ��������� �������, ��������������� ����� � ASCII �������,
// �������� �� � ������ � ���������� ��������� �� ���� ������
// ������ ���������� �� ����������� ROM ���������������, ����������� � ������ ASCII ��������
unsigned char *DS_Match_ROM(unsigned char *ROM){

    // ���������� ��� �������� � �������� ���� �������� ���������� �����������
    unsigned char tempH, tempL;
    // ����������-������� ��� ������� ���� � ROM-������
    unsigned char ROM_Counter=8;
    // ������ ��� �������� ����� � ROM-������
    unsigned char ROM_Buffer;
    // ���������� ��� ASCII ��������, ��������������� ���������� �����������
    // �������� 0x30 ������������� ������ ���� "0,1,2,3..." � ASCII �������
    unsigned char znak=0, doDot=0, posleDot=0, d100=0x30, d10=0x30, d1=0x30, p1000=0x30, p100=0x30, p10=0x30, p1=0x30;
    // ���������� ��� �������������� ������� ����� �������� ���������� �����������
    int pDec=0;
    // ������ � ������� �������� ������ ��� ������ �� ���������
    static unsigned char DispTemp[10];

    // ���� ��������� �����. ����� �������������� ����������� �� 100 �� 750 ��.
    Delay_ms(750);

    // �������:
    // ����� ������������ (����� ���� ��������� �� �����). ���� ��������� ���, ������ ������.
    if(OneWire_Reset()){
        DispTemp[0] = 'N';
        DispTemp[1] = 'o';
        DispTemp[2] = ' ';
        DispTemp[3] = 'S';
        DispTemp[4] = 'e';
        DispTemp[5] = 'n';
        DispTemp[6] = 's';
        DispTemp[7] = 'o';
        DispTemp[8] = 'r';
        DispTemp[9] = '\0';
        return DispTemp;
    }

    // ����� ���������� �� ����� �� ������� ���������� ROM (����� ������������)
    OneWire_Byte_Tx (0x55);

    // ������ �������� ROM ��� ������ ���������������� ���������� �� �����
    while(ROM_Counter--) {
        if(ROM[ROM_Counter*2]>0x39) {
            ROM_Buffer = (ROM[ROM_Counter*2] - 0x37)*16;
        } else {
            ROM_Buffer = (ROM[ROM_Counter*2] - 0x30)*16;
        }
        if(ROM[ROM_Counter*2+1]>0x39) {
            ROM_Buffer += (ROM[ROM_Counter*2+1] - 0x37);
        } else {
            ROM_Buffer += (ROM[ROM_Counter*2+1] - 0x30);
        }
        OneWire_Byte_Tx(ROM_Buffer);
    }
    // ������ ������ ������ � ����������� ����������� (���� ����������� �� �����)
        OneWire_Byte_Tx (0xBE);

    // ������ ���� ���� �������� ���������� �����������
    // ���� ���������� ����������� ��������� � ������� ����� (������� 5 ���)
    // ������������� �������� ����������� ��������� � ������� � ������� ������
    // ������� �������� ����������� ��������� � ������� ����� (������� 4 ����)

    // ������� ���� �������� ���������� �����������
    tempL = OneWire_Byte_Rx ();
    // ������� �������� ���������� �����������
    tempH = OneWire_Byte_Rx ();

    // �������� ������� �� ����� ���������� �������
     if((tempL==0xFF)&&(tempH==0xFF)){
        DispTemp[0] = 'N';
        DispTemp[1] = 'o';
        DispTemp[2] = ' ';
        DispTemp[3] = 'S';
        DispTemp[4] = 'e';
        DispTemp[5] = 'n';
        DispTemp[6] = 's';
        DispTemp[7] = 'o';
        DispTemp[8] = 'r';
        DispTemp[9] = '\0';
        return DispTemp;
    }

    // ���������� ���� ����������� (5 ������� ��� � ������� �����)
    // ���� = 1 �� ������������� �����������
    if(tempH & 128)
        znak = 0x2D;  // ��� ����� "-"  � ASCII
    // ���� 0 - �� �������������
    else
        znak = 0x2B;   // ��� ����� "+"  � ASCII

    // �������� ����� � ������� ����� �������� ���������� �����������
    // � ��������� ����������
    posleDot = (tempL & 0x0F);  // C�������� ������� ����� � ����������
    doDot |= (tempL>>4);        // ��������� ������� 4 ���� ����� �����
    doDot |= ((tempH&0x07)<<4); // ��������� ������� 4 ���� ����� �����

    // ��������� ���������� �������� �� ������� ����� �������� � ������
    // � ��������� � ��������������� ASCII �������

    // ���������� ������� ����� �������� (����� � ASCII)
    while(doDot>=100){
        doDot-=100;
        d100++;
    }
    // ���������� ������� �������� �������� (����� � ASCII)
    while(doDot>=10){
        doDot-=10;
        d10++;
    }
    // ���������� ������� ������ �������� (����� � ASCII)
    while(doDot>=1){
        doDot-=1;
        d1++;
    }

    // ��������������� ������� ����� ����� ������������� ����������
    // ���������� ��� int ������ double
    // ������� ��� ������ �� 1000, ����� ����� ��������� ����� (0,0625) ����� �����

    // ������ ������ ������� ����� ������������� 0,5
    if(posleDot&8)
        pDec = 5000;

    // ������ ������ ������� ����� ������������� 0,25
    if(posleDot&4)
        pDec += 2500;

    // ������ ������ ������� ����� ������������� 0,125
    if(posleDot&2)
        pDec += 1250;

    // ������� ������ ������� ������������� 0,0625
    if(posleDot&1)
        pDec += 625;

    // ��������� ���� ����� ������ ������ (����� � ASCII)

    // ������� ���� �������
    while(pDec>=1000){
        pDec -= 1000;
        p1000++;
    }
    // ����� ���� �������
    while(pDec>=100){
        pDec -= 100;
        p100++;
    }
    // �������� ���� �������
    while(pDec>=10){
        pDec -= 10;
        p10++;
    }
    // �������������� ���� �������
    while(pDec>=1){
        pDec -= 1;
        p1++;
    }

    // ��� ��� ��� ����������� ���������� �������� ���������� �����������
    // �� ���������� �������������� �������� � �������� � ����� 0x30,
    // �� �� ����� �������� �������� ���� � ASCII ����.

    // ������� �� � ������
    DispTemp[0] = znak;    // ����
    DispTemp[1] = d100;    // ����� �����, �����
    DispTemp[2] = d10;     // ����� �����, �������
    DispTemp[3] = d1;      // ����� �����, �������
    DispTemp[4] = '.';     // ���������� �����
    DispTemp[5] = p1000;   // ������� �����, �������
    DispTemp[6] = p100;    // ������� �����, �����
    DispTemp[7] = p10;     // ������� �����, ������
    DispTemp[8] = p1;      // ������� �����, ������� �����
    DispTemp[9] = '\0';    // ����� ���������� �������

    // ���������� ��������� �� ������
    return DispTemp;
}


//##############################################################################
//
// ���� ������������� �������, ����������� � 12 ������ ����������, � ������:
// ����������� - �������� ����������� � double
// ������ - ������� ���������� �� ROM � �������� ������
// ����� - ������������� ����������, ����� ������ �����, �������� ������ �����,
// ����� - Match_ROM (�������� ���� ROM � ��������), Read_ROM (�������� ���� ROM)
//
//##############################################################################



// ������� ��������� ��������� �������, ��������������� ����� � ��������� ������
// ������ ���������� �� ����������� ROM ���������������, ����������� � ������ ASCII ��������
double DS_ReadTemp_byROM(unsigned char *ROM){

    // ������������� ���������� ����������� � ��������� �������
    double temp_buffer = 0.;
    // ���������� ��� �������� � �������� ���� �������� ���������� �����������
    unsigned char tempH, tempL;
    // ����������-������� ��� ������� ���� � ROM-������
    unsigned char ROM_Counter=8;
    // ������ ��� �������� ����� � ROM-������
    unsigned char ROM_Buffer;

    // �������:
    // ����� ������������ (����� ���� ��������� �� �����). ���� ��������� ���, ������ ������.
    if(OneWire_Reset())
        return -666.;

    // ����� ���������� �� ����� �� ������� ���������� ROM (����� ������������)
    OneWire_Byte_Tx (0x55);

    // ������ �������� ROM ��� ������ ���������������� ���������� �� �����
    while(ROM_Counter--) {
        if(ROM[ROM_Counter*2]>0x39) {
            ROM_Buffer = (ROM[ROM_Counter*2] - 0x37)*16;
        } else {
            ROM_Buffer = (ROM[ROM_Counter*2] - 0x30)*16;
        }
        if(ROM[ROM_Counter*2+1]>0x39) {
            ROM_Buffer += (ROM[ROM_Counter*2+1] - 0x37);
        } else {
            ROM_Buffer += (ROM[ROM_Counter*2+1] - 0x30);
        }
        OneWire_Byte_Tx(ROM_Buffer);
    }
    // ������ ������ ������ � ����������� ����������� (���� ����������� �� �����)
        OneWire_Byte_Tx (0xBE);

    // ������ ���� ���� �������� ���������� �����������
    // ���� ���������� ����������� ��������� � ������� ����� (������� 5 ���)
    // ������������� �������� ����������� ��������� � ������� � ������� ������
    // ������� �������� ����������� ��������� � ������� ����� (������� 4 ����)

    // ������� ���� �������� ���������� �����������
    tempL = OneWire_Byte_Rx ();
    // ������� �������� ���������� �����������
    tempH = OneWire_Byte_Rx ();

    // �������� ������� �� ����� ���������� �������
     if((tempL==0xFF)&&(tempH==0xFF)){
        return 666.;
    }

    // ���������� ���� ����������� (5 ������� ��� � ������� �����)
    // ���� = 1 �� ������������� �����������
    if(tempH & 128)
        temp_buffer = -0.;
    // ���� 0 - �� �������������
    else
        temp_buffer = +0.;

    // ��������� �������� � ����� � ��������� �������
    temp_buffer += (((tempH & 0x07)<<4) + ((tempL & 0xF0) >>4));
    if(tempL & 0x01)     temp_buffer += 0.5;
    if(tempL & 0x02)     temp_buffer += 0.25;
    if(tempL & 0x04)     temp_buffer += 0.125;
    if(tempL & 0x08)     temp_buffer += 0.0625;

    // ���������� ��������� �� ������
    return temp_buffer;
}



// ������� ������� n ���� ������ (LSB) ����������  �� �����,
// ��������� �� ROM � ASCII
// ���������� 1 ���� ��� ����������, ����� 0
unsigned char OneWire_Match_ROM_Tx(unsigned char *ROM, unsigned char *dat, unsigned char n){

    // �������� �������� ��������� ���� ROM-������ � ������������ ������
    unsigned char buffer;
    // ������� ��� ������� ��������� ���� � ROM-������ � ������� ������
    unsigned char counter=8;

    // ����� (����� ���� ��������� �� �����)
    // ���� ��������� ���, ������ ������ ��������� 1
    if(OneWire_Reset())
        return 1;

    // ������� ������ ���������� �� ����� �� ������� ���������� ROM-������
    OneWire_Byte_Tx (0x55);

    // �������� ROM � ����� (����������� ������� ���� ��������)
    while(Counter--) {

        // ������� ASCII ������
        if(ROM[Counter*2]>0x39)
            Buffer = (ROM[Counter*2] - 0x37) * 16; // A..F
        else
            Buffer = (ROM[Counter*2] - 0x30) * 16; // 0..9

        //������� ASCII ������
        if(ROM[Counter*2+1]>0x39)
            Buffer += (ROM[Counter*2+1] - 0x37); // A..F
        else
            Buffer += (ROM[Counter*2+1] - 0x30); // 0..9

        // �������� ��������������� �����
        OneWire_Byte_Tx(Buffer);
    }

    // �������� ������ � ����� (��������, ���������� ����� � �������)
    counter = n;
    while(counter--) {
        Buffer = dat[counter];
        OneWire_Byte_Tx(Buffer);
    }
    // ���� �������� ������ �������, ���������� 0
    return 0;
}


// 3.1 OneWire_Slave_Presence
// ������������ �������� ������������� ����������� �� ����� (����������� �������)
// ���������� 1 ���� ����������� ������������, ����� 0
unsigned char OneWire_Slave_Presence(void){

    // ������ ������ � ����� ����������� ������������ �������� ������ �����
    // ��� ������ ������� ���������� �������� ����� �� ����� 480 ���
    // ����� ������ ����� ������� ���������� ���������� ���� ����������� �� �����
    // ��� ����������� ����������� ����� ���������� ����� ������� �����������,
    // ����� �������� ������� ���������� (����������� ��������� - �� 60-240 ���)

    // ������� ������������ ������ ����������
    unsigned char counter = 1;
    // ��������� �������� ������ � �������������.
    // ���������� ��� DS18B20: '0'-502 ���, '1'-30 ���, '0'-114 ���.
    // ������������ � ����� ������: '0'-502 ���, '1'-30 ���, '0'-102 ���.
    // ���� ������� ���������������� 0, �� ������ �� �������� (?!)
    // ���� ������� �������� � ������ (�� ��������� ��� 1 � 45) ������ 13
    // ������������ ���������� �������� ����������� �� "�������" ��������� �������
    // �������, ������������ '0' ��������� ������� �� �������� 10 ������, � �� ���������� 85 ���
    // ����� ������������ ������ 502/85=59 �� �������� (� delay_us(1); � PORTC = counter;)

    // ��������� ����� ��� ����
    DS_TRIS |= (1<<DS);
    //���� ��������� ������ �������, ������� ������������ ���������� �����
    while(!(DS_PORT&(1<<DS))){
        if(counter>60)
            counter=60;
        else
            counter++;
        delay_us(1);
    }
    // ���� ���������� ������������ ������� ���� ��� ������, ���������� 0
     if(counter < 45)     //counter<143 - works, counnter<144 - don't work (��� �����)
        return counter;//0;

    // ����� ������������ ����������� �� �����
    // ������� ��������, ����� ��������� ������ ���� �������� ����� ������ (�� �����������)
    Delay_us(20);

    // �������� ����� ��� ����� � ���������
    DS_TRIS &= ~(1<<DS);
    DS_PORT &= ~(1<<DS);
    // ����������� ������������ ������������� �����������
    // � ����������� ����� (��������� ��� ����)
    Delay_us(100);
    DS_TRIS |= (1<<DS);
    
    // �������, ���� ��� ������ �� �������� �����
    while(!(DS_PORT|=(1<<DS)));
    
    // ����� ����������� ������������� ����������� �� ����� ���������� 1
    return 0;//1;

}


// 3.2 OneWire_Slave_Byte_Rx
// ����� ������ ����� (����������� �������)
unsigned char OneWire_Slave_Byte_Rx (void){

    // ����������: �������� ���� � ������� ���������� �������� ���
    unsigned char temp=0, i;

    // ���� ��������� ������ �����, ������� � �������� ����
    for(i=0;i<8;i++){

        // �������� ������� ���� �������������� � ���� ����� (��)
        // (��� ������� ������� ������������� 60-120 ��� + 1 ��� ����� ���� ��)
        // ������ �� ���������� ���������� ����� (1-15 ���)
        // ������ ��� ������������ ���������� ����� ����� ��������� �����
        // ����� ������ ���� �����. �������� ���� = 0 ������������ ����������
        // ����� ������� �����������, �������� ���� = 1 - ����������� ���������

        // ����������� ����� ��� ����
        DS_TRIS |= (1<<DS);
        // ������� ������ ���������
        while(DS_PORT&(1<<DS)){};
        // ���������� ��������� ������ �� (����� 30 ���) � ��������� ������
        Delay_us(30);

        // ������ ��� ������
        // ���� = 1, �� ������������� � �������� ������� ������ ������� ����� 1
        // ���� = 0, �� ��� � ��������� � ������� ���� 0, � ������� ����� ���������
        if(DS_PORT & (1<<DS))
            temp |= 128;
        else
            while(!(DS_PORT&(1<<DS))){};

        // ���� ��������� �� ��������� ���,
        // �� �������� �������� ������� ���������� ����� ���������� ����
        if(i<7)
            temp>>=1;
    }
    // �� ��������� ������ 8 ���, ���������� �������������� ����
    return temp;
}



// 3.3 OneWire_Slave_Byte_Tx
// �������� ������ ����� (����������� �������)
void OneWire_Slave_Byte_Tx (unsigned char dat){

    // ������� ���������� ���������� ���
    unsigned char i;

    // ���� �������� �������� �����, ������� � �������� ����
    for(i=0;i<8;i++){

        // �������� ������� ���� �������������� � ���� ����� (��)
        // (��� ������� ������� ������������� 60-120 ��� + 1 ��� ����� ���� ��)
        // ������ �� ���������� ���������� ����� (1-15 ���)
        // ������ ��� ������������ ���������� ����� ����� ��������� �����
        // ����� ������ ���� �����. �������� ���� = 0 ���������� �������
        // �������� ��������� ����� � ������ ���������, ��� �������� ���� = 1

        // ����������� ����� ��� ����
        DS_TRIS |= (1<<DS);

        // ���� ������������ ��� = 1
        if(dat &(1<<i)){
            // ������� ������ ���������
            while(DS_PORT&(1<<DS));
            // ���������� ��������� ������ �� (����� 30 ���)
            // � ���� �������������� ��������� ������� (�������������� ����������)
            // ������� ����� ���������
            while(!(DS_PORT&(1<<DS)));
        }
        // ���� ������������ ��� = 0
        else{
            // ������� ������ ���������
            while(DS_PORT&(1<<DS));
            // ����������� ����� �� ����� � ��������� ������� ����������� ����
            DS_TRIS &= ~(1<<DS);
            DS_PORT &= ~(1<<DS);
            // ��� �������� ���� ����� ����������� � ����� �������
            // (����� �� 30 ���), � ����� �����������
            Delay_us(60); // ������� ������� �������� <=78 ����� �� �����
            DS_TRIS |= (1<<DS);
            // ��������� ����� �� �������������� ���������� �� ����� (����� 30 ���)
            // ������� ����� ���������
            while(!(DS_PORT&(1<<DS)));
        }
    }
}



// 3.4 OneWire_Slave_Read_ROM
// ������� �������� ROM-������ (����������� �������)
// ���������� ����� ������� ����� ������ ������� ������� Read_ROM (0x33)
void OneWire_Slave_Read_ROM (unsigned char *ROM){

    // ������� ��� ������� ��������� ���� � ROM-������
    unsigned char ROM_Counter=8;
    // ������ ��� �������� ��������� ���� ROM-������
    unsigned char ROM_Buffer;

    // �������� ROM � ����� (����������� ������� ���� ��������)
    while(ROM_Counter--) {

        // ������� ASCII ������
        if(ROM[ROM_Counter*2]>0x39)
            ROM_Buffer = (ROM[ROM_Counter*2] - 0x37) * 16; // A..F
        else
            ROM_Buffer = (ROM[ROM_Counter*2] - 0x30) * 16; // 0..9

        //������� ASCII ������
        if(ROM[ROM_Counter*2+1]>0x39)
            ROM_Buffer += (ROM[ROM_Counter*2+1] - 0x37); // A..F
        else
            ROM_Buffer += (ROM[ROM_Counter*2+1] - 0x30); // 0..9

        // �������� ��������������� �����
        OneWire_Slave_Byte_Tx(ROM_Buffer);
    }
}


// 3.5 OneWire_Slave_Match_ROM
// ������� ��������� ROM-������ (����������� �������)
// ���������� ����� ������� ����� ������ ������� ������� Match_ROM(0x55)
// ���� �������� ROM ��������� � ��������, ���������� 1, ����� ������
// ����� ���������� 0, ����� ������� �� ���������� ������ �����
unsigned char OneWire_Slave_Match_ROM (unsigned char *ROM){

    // ������� ��� ������� ��������� ���� � ROM-������
    unsigned char ROM_Counter=8;
    // ������ ��� �������� ��������� ���� ROM-������
    unsigned char ROM_Buffer;

    // �������� ROM � ����� (����������� ������� ���� ��������)
    while(ROM_Counter--) {
    
        // ������� ASCII ������
        if(ROM[ROM_Counter*2]>0x39)
            ROM_Buffer = (ROM[ROM_Counter*2] - 0x37) * 16; // A..F
        else
            ROM_Buffer = (ROM[ROM_Counter*2] - 0x30) * 16; // 0..9
        
        //������� ASCII ������
        if(ROM[ROM_Counter*2+1]>0x39)
            ROM_Buffer += (ROM[ROM_Counter*2+1] - 0x37); // A..F
        else
            ROM_Buffer += (ROM[ROM_Counter*2+1] - 0x30); // 0..9

        // ���� �������� ��������� ����� ROM �� ��������� � ��������,
        // ��������� ������� �����, ���������� 0
        if(OneWire_Slave_Byte_Rx()!=ROM_Buffer)
            return 0;
    }
    // ���� �������� ��������� ROM ������� � ��������, ���������� 1
    return 1;
}
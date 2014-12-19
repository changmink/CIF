#include<cif.h>
//주소설정 
void set_f0_addr_y(unsigned int addr){
	CIF_F0_ADDR_Y = addr;
}
void set_f0_addr_uv(unsigned int addr){
	CIF_F0_ADDR_UV = addr;
}
void set_f1_addr_y(unsigned int addr){
	CIF_F1_ADDR_Y = addr;
}
void set_f1_addr_uv(unsigned int addr){
	CIF_F1_ADDR_UV = addr;
}
//주소 받기 
unsigned int get_f0_addr_y(){
	return CIF_F0_ADDR_Y;
}
unsigned int get_f0_addr_uv(){
	return CIF_F0_ADDR_UV;
}
unsigned int get_f1_addr_y(){
	return CIF_F1_ADDR_Y;
}
unsigned int get_f1_addr_uv(){
	return CIF_F1_ADDR_UV;
}

unsigned int get_last_line(){
	unsigned int last_line;
	last_line = CIF_LAST_LINE & 0x3fff;
	return last_line;
}
unsigned int get_last_pixel(){
	unsigned int last_pixel;
	last_pixel = CIF_LAST_PIX & 0x7fff;
	return last_pixel;
}
unsigned int current_dst_address(){
	return CIF_CUR_DST
}
//프레임 수
unsigned short get_frame_num(){
	unsigned short number;
	number = (CIF_FRAME_STATUS & ffff0000)>> 16; 
	return  number;
}

//
/*set input mode
	YUV			0
	PAL			2
	NTSC		3
	RAW			4
	JPEG		5
	MIPI		6		*/
void set_input_mode(unsigned char mode){
	CIF_FORMAT = (CIF_FORMAT & 0x1C) | (mode << 2)
}

//화면을 잘라내는 함수 start 0,0이면 사이즈도 정할수 있음 
void cif_crop(int start_x ,int start_y, int set_x, int set_y){
    CIF_CROP = start_x + (start_y << 16);
    CIF_SET_SIZE = set_x + (set_y << 16);
}
void cif_set_size(int set_x, int set_y){
	cif_crop(0,0,set_x,set_y);
}

void cif_scale(){
	CIF_SCL_CTRL = 0x1 << 4;//YUV 16 bit bypass
}


void cif_set_burst_length(char length){
	CIF_CTRL = (CIF_CTRL & (0x00 << 12)) |  (length << 12);
}

void cif_set_fomat_default(){
	CIF_FORMAT = 0x0 + //VSYNC active low
				(0x0 << 1) + //HREF active high
				(0x0 << 2) + //YUV(YCbCr)
				(0x0 << 5) + //UYVY
				(0x1 << 7) + //420 input
				(0x0 << 8) + //UV even line output
				(0x0 << 9) + // ccir(bt656) odd field first
				(0x0 << 10) + //not jpeg mode
				(0x0 << 11) + //raw data width
				(0x1 << 16) + //420 output
				(0x0 << 17) + //UV even line output
				(0x0 << 18) + //little endian
				(0x0 << 19); //UVUV storage order
}
void cif_set_unknown(){
	CIF_FIFO_ENTRY = 0 + //y FIFO entry clear
					 (0 << 8); //uv FIFO entry claer
	CIF_CIF_VIR_LINE_WIDTH = 0; //reset value in datasheet
}
/
#define F0_UV/임의의 주소에 쓴다고 가정
#define F0_Y
#define F1_UV
#define F1_Y
#define IMAGE_SIZE		720*480
void cif_capture(char ){

	

	

}
/* 세미나 때 보여줄 부분 */
//HW 적 부분을 하나의 함수로설정
void cif_init_camera(unsigned int * address){
	cif_disable();

	//시작점 설정
	CIF_CROP = 0 | 0 << 16;
	//사이즈 설정
	cif_set_size(720, 480);

	CIF_CTRL =	CIF_CTRL |
				(0x1  << 1) | //ping pong
				(0x07 << 12);//버스트는 default값 

	//인터럽트 설정	
	CIF_INTEN = 0x1 + //frame end
					(0x0 << 1) + //line end
					(0x0 << 2) + //line error
					(0x0 << 3) + //pixel error
					(0x0 << 6);	 //Bus error
	
	//포맷 설정
	CIF_FORMAT = 0x0 + //VSYNC active low
					(0x0 << 1) + //HREF active high
					(0x0 << 2) + //YUV(YCbCr)
					(0x0 << 5) + //UYVY
					(0x1 << 7) + //420 input
					(0x0 << 8) + //UV even line output
					(0x0 << 9) + // ccir(bt656) odd field first
					(0x0 << 10) + //not jpeg mode
					(0x0 << 11) + //raw data width
					(0x1 << 16) + //420 output
					(0x0 << 17) + //UV even line output
					(0x0 << 18) + //little endian
					(0x0 << 19); //UVUV storage order
	
	//write 0 clear
	CIF_F_STATUS = 0; 

	//no scale
	CIF_CIF_SCL_CTRL = 0;				  

	//주소 초기화 (원래는 메모리의 특정주소로 해야한다.)
	CIF_F0_ADDR_Y = address;
	CIF_F0_ADDR_UV = address;
	CIF_F1_ADDR_Y = address;
	CIF_F1_ADDR_UV = address;

	cif_enable();

}
void cif_enable(){
	CIF_CTRL =	CIF_CTRL | 0x1 
}
void cif_disable(){
	CIF_CTRL = CIF_CTRL | 0x0;
}
void cif_set_one_frame(){
	CIF_CTRL = CIF_CTRL | (0x0  << 1);
}
void cif_set_pingpong(){
	CIF_CTRL = CIF_CTRL | (0x1  << 1);
}


void cif_capture_next(char flag){

	//기본 포맷
	cif_set_fomat_default();

	//이미지 사이즈 
	cif_set_size(720, 480);

	//주소 설정 
	set_f0_addr_y(F0_Y);
	set_f1_addr_y(F1_Y);
	set_f0_addr_uv(F0_UV);
	set_f1_addr_uv(F1_UV);
	
	//인터럽트 시작
	set_frame_end_INT();

	//캡쳐 시작
	cif_enable();

}
void set_frame_end_INT(){
	CIF_INTEN = CIF_INTEN | 1;
}
void __irq ISR(void)
{
	set_f0_addr_y(F0_Y + IMAGE_SIZE);
	set_f1_addr_y(F1_Y + IMAGE_SIZE);
	set_f0_addr_uv(F0_UV + IMAGE_SIZE);
	set_f1_addr_uv(F1_UV + IMAGE_SIZE);
	CIF_INTEN = 0;
}





#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void encrypt(char*, char*, char*);
void decrypt(char*, char*);
void str2bin(char*, int*);
void bin2str(int*, char*);
void initial_permutation(int*, int*);
void expansion_permutation(int*, int*);
void key64to56(int*, int*);
void key_shift(int*, int);
void key56to48(int*, int*);
int xor_operation(int, int);
void compression_permutation(int*, int*);
void straight_permutation(int*, int*);
void print_round(int*, int*, int);
void inverse_initial_permutation(int*, int*);
void bin2hex(int*, char*);
void hex2bin(char*, int*);

// Initial Permutation Table
int IP_Box[] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

// Inverse Initial Permutation Table
int IIP_Box[] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25
};

// Parity Bit Drop Table
int PBD_Box[] = {
	57, 49, 41, 33, 25, 17, 9, 1,
	58, 50, 42, 34, 26, 18, 10, 2,
	59, 51, 43, 35, 27, 19, 11, 3,
	60, 52, 44, 36, 63, 55, 47, 39,
	31, 23, 15, 7, 62, 54, 46, 38,
	30, 22, 14, 6, 61, 53, 45, 37,
	29, 21, 13, 5, 28, 20, 12, 4
};

// Expansion Permutation Table
int EP_Box[] = {
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1
};

// Key Shift Table
int Shift_Box[] = {
	1, 1, 2, 2,
	2, 2, 2, 2,
	1, 2, 2, 2,
	2, 2, 2, 1
};

// Compression Permutation Table
int CP_Box[] = {
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

// Straight Permutation Table
int SP_Box[] = {
	16, 7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2, 8, 24, 14,
	32, 27, 3, 9,
	19, 13, 30, 6,
	22, 11, 4, 25
};

// S-Box Table
int S1_Box[4][16] = {
	{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
	{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
	{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
	{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
};

int S2_Box[4][16] = {
	{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
	{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
	{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
	{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
};

int S3_Box[4][16] = {
	{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
	{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
	{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
	{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
};

int S4_Box[4][16] = {
	{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
	{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
	{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
	{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
};

int S5_Box[4][16] = {
	{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
	{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
	{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
	{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
};

int S6_Box[4][16] = {
	{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
	{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
	{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
	{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
};

int S7_Box[4][16] = {
	{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
	{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
	{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
	{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
};

int S8_Box[4][16] = {
	{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
	{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
	{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
	{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
};

// Round Key
int RoundKey[16][48];

int main() {
	char plaintext[9], ciphertext[17], key[9], plain[9];
	
	printf("평문(Plaintext) 입력 (8byte): ");
	gets_s(plaintext, sizeof(plaintext));
	if (strlen(plaintext) != 8) {
		printf("문자 8개 입력!\n");
		exit(1);
	}

	printf("키(Key) 입력 (8byte): ");
	gets_s(key, sizeof(key));
	if (strlen(key) != 8) {
		printf("문자 8개 입력!\n");
		exit(1);
	}
	
	encrypt(plaintext, ciphertext, key);
	decrypt(ciphertext, plain);

	return 0;
}

void encrypt(char* pt, char* ct, char* key) {
	int b_pt[64], i_pt[64], b_key[64], b_ct[64], i_ct[64];
	int b_left[32], b_right[32], bp_right[48];
	int key56bit[56], key48bit[48];
	int r_xor[48], rc_xor[32], rs_xor[32], lr_xor[32];
	int round = 16;

	// 평문을 2진수로 변환
	str2bin(pt, b_pt);

	// 키를 2진수로 변환
	str2bin(key, b_key);

	// 평문 초기 치환 (Initial Permutation Table)
	initial_permutation(b_pt, i_pt);

	// 평문을 32bit씩 왼쪽, 오른쪽으로 나눔
	for (int i = 0; i < 32; i++) {
		b_left[i] = i_pt[i];
		b_right[i] = i_pt[i + 32];
	}

	// 키 압축 (Parity Bit Drop Table, 64bit -> 56bit)
	key64to56(b_key, key56bit);

	for (int i = 0; i < round; i++) {
		// 오른쪽 확장 치환 (Expansion Permutation Table, 32bit -> 48bit)
		expansion_permutation(b_right, bp_right);
		// 키 shift 연산 (Key Shift Table)
		key_shift(key56bit, i);
		// 키 압축 (Compression Permutation Table, 56bit -> 48bit)
		key56to48(key56bit, key48bit);

		// Round Key 저장
		for (int j = 0; j < sizeof(key48bit) / sizeof(int); j++) {
			RoundKey[i][j] = key48bit[j];
		}

		// 오른쪽과 키 XOR 연산
		for (int i = 0; i < sizeof(key48bit) / sizeof(int); i++) {
			r_xor[i] = xor_operation(bp_right[i], key48bit[i]);
		}
		// XOR 결과 압축 (S-Box Table, 48bit -> 32bit)
		compression_permutation(r_xor, rc_xor);
		// 압축 결과 치환 (Straight Permutation Table)
		straight_permutation(rc_xor, rs_xor);
		// 왼쪽과 오른쪽 XOR 연산
		for (int i = 0; i < sizeof(rs_xor) / sizeof(int); i++) {
			lr_xor[i] = xor_operation(b_left[i], rs_xor[i]);
		}
		// round 횟수면 빠져나옴
		if (i + 1 == round) {
			break;
		}
		// 왼쪽 -> 오른쪽, 오른쪽 -> 왼쪽
		for (int i = 0; i < 32; i++) {
			b_left[i] = b_right[i];
			b_right[i] = lr_xor[i];
		}
		// 라운드마다 출력
		print_round(b_left, b_right, i);
	}

	// 왼쪽 -> 오른쪽, 오른쪽 -> 왼쪽 하지 않고 나옴
	for (int i = 0; i < 32; i++) {
		b_ct[i] = lr_xor[i];
		b_ct[i + 32] = b_right[i];
	}
	// 평문 역 치환 (Inverse Initial Permutation Table)
	inverse_initial_permutation(b_ct, i_ct);
	// 2진수를 암호문으로 변환
	bin2hex(i_ct, ct);

	printf("암호문(Ciphertext) 출력: %s\n", ct);
}

void decrypt(char* ct, char* pt) {
	int b_ct[64], i_ct[64], b_pt[64], i_pt[64];
	int b_left[32], b_right[32], bp_right[48];
	int key48bit[48];
	int r_xor[48], rc_xor[32], rs_xor[32], lr_xor[32];
	int round = 16;

	// 암호문(16진수)을 2진수로 변환
	hex2bin(ct, b_ct);

	// 암호문 초기 치환 (Initial Permutation Table)
	initial_permutation(b_ct, i_ct);

	// 평문을 32bit씩 왼쪽, 오른쪽으로 나눔
	for (int i = 0; i < 32; i++) {
		b_left[i] = i_ct[i];
		b_right[i] = i_ct[i + 32];
	}

	for (int i = 0; i < round; i++) {
		// 오른쪽 확장 치환 (Expansion Permutation Table, 32bit -> 48bit)
		expansion_permutation(b_right, bp_right);
		
		// Round Key 가져옴
		for (int j = 0; j < sizeof(RoundKey[0]) / sizeof(int); j++) {
			key48bit[j] = RoundKey[round - i - 1][j];
		}

		// 오른쪽과 키 XOR 연산
		for (int i = 0; i < sizeof(key48bit) / sizeof(int); i++) {
			r_xor[i] = xor_operation(bp_right[i], key48bit[i]);
		}
		// XOR 결과 압축 (S-Box Table, 48bit -> 32bit)
		compression_permutation(r_xor, rc_xor);
		// 압축 결과 치환 (Straight Permutation Table)
		straight_permutation(rc_xor, rs_xor);
		// 왼쪽과 오른쪽 XOR 연산
		for (int i = 0; i < sizeof(rs_xor) / sizeof(int); i++) {
			lr_xor[i] = xor_operation(b_left[i], rs_xor[i]);
		}
		// round 횟수면 빠져나옴
		if (i + 1 == round) {
			break;
		}
		// 왼쪽 -> 오른쪽, 오른쪽 -> 왼쪽
		for (int i = 0; i < 32; i++) {
			b_left[i] = b_right[i];
			b_right[i] = lr_xor[i];
		}
		// 라운드마다 출력
		print_round(b_left, b_right, i);
	}

	// 왼쪽 -> 오른쪽, 오른쪽 -> 왼쪽 하지 않고 나옴
	for (int i = 0; i < 32; i++) {
		b_pt[i] = lr_xor[i];
		b_pt[i + 32] = b_right[i];
	}
	// 암호문 역 치환 (Inverse Initial Permutation Table)
	inverse_initial_permutation(b_pt, i_pt);
	
	// 2진수를 평문으로 변환
	bin2str(i_pt, pt);

	printf("평문(Plaintext) 출력: %s\n", pt);
}

void str2bin(char* in, int* out) {
	for (int i = 0; i < strlen(in); i++) {
		char val = in[i];

		for (int j = 0; j < 8; j++) {
			char shift_byte = 0x01 << (7 - j);
			out[i * 8 + j] = (shift_byte & val) ? 1 : 0;
		}
	}
}

void bin2str(int* in, char* out) {
	int arr8bit[8];

	for (int i = 0, idx = 0; i < 8; i++, idx++) {
		for (int j = 0; j < 8; j++) {
			arr8bit[j] = in[i * 8 + j];
		}

		int dec = 0, pos = 0;
		for (int j = sizeof(arr8bit) / sizeof(int) - 1; j >= 0; j--) {
			if (arr8bit[j] == 1) {
				dec += 1 << pos;
			}
			pos++;
		}

		sprintf((char*)(out + idx), "%c", dec);
	}
}

void initial_permutation(int* in, int* out) {
	for (int i = 0; i < 64; i++) {
		out[i] = in[IP_Box[i] - 1];
	}
}

void expansion_permutation(int* in, int* out) {
	for (int i = 0; i < 48; i++) {
		out[i] = in[EP_Box[i] - 1];
	}
}

void key64to56(int* in, int* out) {
	for (int i = 0; i < 56; i++) {
		out[i] = in[PBD_Box[i] - 1];
	}
}

void key_shift(int* in, int round) {
	int num = Shift_Box[round];
	int left[28], right[28];

	for (int i = 0; i < 28; i++) {
		left[i] = in[i];
		right[i] = in[i + 28];
	}

	if (num == 1) {
		int l_tmp = left[0];
		for (int i = 0; i < 27; i++) {
			left[i] = left[i + 1];
		}
		left[27] = l_tmp;

		int r_tmp = right[0];
		for (int i = 0; i < 27; i++) {
			right[i] = right[i + 1];
		}
		right[27] = r_tmp;
	}
	else {
		int l_tmp1 = left[0], l_tmp2 = left[1];
		for (int i = 0; i < 26; i++) {
			left[i] = left[i + 2];
		}
		left[26] = l_tmp1, left[27] = l_tmp2;

		int r_tmp1 = right[0], r_tmp2 = right[1];
		for (int i = 0; i < 26; i++) {
			right[i] = right[i + 2];
		}
		right[26] = r_tmp1, right[27] = r_tmp2;
	}

	for (int i = 0; i < 28; i++) {
		in[i] = left[i];
		in[i + 28] = right[i];
	}
}

void key56to48(int* in, int* out) {
	for (int i = 0; i < 48; i++) {
		out[i] = in[CP_Box[i] - 1];
	}
}

int xor_operation(int a, int b) {
	return (a ^ b);
}

void compression_permutation(int* in, int* out) {
	int col[2], row[4];

	for (int i = 0; i < 8; i++) {
		col[0] = in[i * 6];
		col[1] = in[i * 6 + 5];

		int d_col = 0, c_pos = 0;
		for (int j = sizeof(col) / sizeof(int) - 1; j >= 0; j--) {
			if (col[j] == 1) {
				d_col += 1 << c_pos;
			}
			c_pos++;
		}

		row[0] = in[i * 6 + 1];
		row[1] = in[i * 6 + 2];
		row[2] = in[i * 6 + 3];
		row[3] = in[i * 6 + 4];

		int d_row = 0, r_pos = 0;
		for (int j = sizeof(row) / sizeof(int) - 1; j >= 0; j--) {
			if (row[j] == 1) {
				d_row += 1 << r_pos;
			}
			r_pos++;
		}

		int sbp = 0;
		if (i == 0) {
			sbp = S1_Box[d_col][d_row];
		}
		else if (i == 1) {
			sbp = S2_Box[d_col][d_row];
		}
		else if (i == 2) {
			sbp = S3_Box[d_col][d_row];
		}
		else if (i == 3) {
			sbp = S4_Box[d_col][d_row];
		}
		else if (i == 4) {
			sbp = S5_Box[d_col][d_row];
		}
		else if (i == 5) {
			sbp = S6_Box[d_col][d_row];
		}
		else if (i == 6) {
			sbp = S7_Box[d_col][d_row];
		}
		else {
			sbp = S8_Box[d_col][d_row];
		}

		int trans[4] = { 0, };
		int idx = 0;

		while (sbp > 0) {
			trans[3 - idx++] = sbp % 2;
			sbp /= 2;
		}

		for (int j = 0; j < sizeof(trans) / sizeof(int); j++) {
			out[i * 4 + j] = trans[j];
		}
	}
}

void straight_permutation(int* in, int* out) {
	for (int i = 0; i < 32; i++) {
		out[i] = in[SP_Box[i] - 1];
	}
}

void print_round(int* l, int* r, int num) {
	char ic[17];
	int b_ic[64];
	int arr8bit[8];

	for (int i = 0; i < 32; i++) {
		b_ic[i] = l[i];
		b_ic[i + 32] = r[i];
	}

	for (int i = 0, idx = 0; i < 8; i++, idx += 2) {
		for (int j = 0; j < 8; j++) {
			arr8bit[j] = b_ic[i * 8 + j];
		}

		int dec = 0, pos = 0;
		for (int j = sizeof(arr8bit) / sizeof(int) - 1; j >= 0; j--) {
			if (arr8bit[j] == 1) {
				dec += 1 << pos;
			}
			pos++;
		}

		sprintf((char*)(ic + idx), "%02X", dec);
	}

	printf("Round %d : %s\n", (num + 1), ic);
}

void inverse_initial_permutation(int* in, int* out) {
	for (int i = 0; i < 64; i++) {
		out[i] = in[IIP_Box[i] - 1];
	}
}

void bin2hex(int* in, char* out) {
	int arr8bit[8];

	for (int i = 0, idx = 0; i < 8; i++, idx += 2) {
		for (int j = 0; j < 8; j++) {
			arr8bit[j] = in[i * 8 + j];
		}

		int dec = 0, pos = 0;
		for (int j = sizeof(arr8bit) / sizeof(int) - 1; j >= 0; j--) {
			if (arr8bit[j] == 1) {
				dec += 1 << pos;
			}
			pos++;
		}

		sprintf((char*)(out + idx), "%02X", dec);
	}
}

void hex2bin(char* in, int* out) {
	int val = 0;
	for (int i = 0; i < strlen(in); i++) {
		if (isalpha(in[i])) {
			val = in[i] - 55;
		}
		else {
			val = in[i] - 48;
		}

		for (int j = 0; j < 4; j++) {
			char shift_byte = 0x01 << (3 - j);
			out[i * 4 + j] = (val & shift_byte) ? 1 : 0;
		}
	}
}

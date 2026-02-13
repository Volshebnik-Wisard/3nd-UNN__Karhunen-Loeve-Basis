
// KarhunenLoeveDlg.h: файл заголовка
//

#pragma once
#include "Drawer.h"
#include <vector>
#include <iostream>
#include <time.h>
#include "afxcmn.h"

// Диалоговое окно CKarhunenLoeveDlg
class CKarhunenLoeveDlg : public CDialogEx
{
	// Создание
public:
	CKarhunenLoeveDlg(CWnd* pParent = nullptr);	// стандартный конструктор

	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SVD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


	// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGeneratesign();
	afx_msg void OnBnClickedDrawv();
	afx_msg void OnBnClickedDrawu();
	double A1;
	double A2;
	double A3;
	double F1;
	double F2;
	double F3;
	double PHASE1;
	double PHASE2;
	double PHASE3;
	double N;
	double FD;
	int P;
	int NUMBERV;
	int NUMBERU;
	Drawer
		Sign,
		V,
		U,
		SingularValues;
	double FindMin(vector <double> data, int N);
	double FindMax(vector <double> data, int N);
	void CreateSignal();
	vector <double> Signal;
	vector <double> keys;
	double xmax;
	double ymin;
	double ymax;
	vector <double> rxx;
	void Autocorrelation();
	vector <double> rnew;
	vector <vector<double>> MatrixR;
	double* MatrixA = new double[N * N];
	double* UVEC = new double[N * N];
	double* VVEC = new double[N * N];
	double* SigmaVEC = new double[N];

	double** conToN(double* buf);
	int
		rows,
		col;
	double** M;
	double** conToN(double* buf, bool flag);
	double** Mult(double** A, double** B, int rowsB, int colB, int rows, int cols);
	void Trans(double** M);
	bool fl;
	vector <double> yrec;
	double* yi = new double[N];
	vector <double> SIGMA;

	vector <double> uvec;
	vector <double> puvec;

	vector <double> vvec;
	vector <double> pvvec;

	void GetVecV();
	void GetVecU();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	CSpinButtonCtrl SpinV;
	CSpinButtonCtrl SpinU;

};



class Matrix
{
private:
	int
		rows,
		col;
	double** M; //память под матрицу


public:
	//конструкторы
	Matrix();
	Matrix(int rows, int col);
	Matrix(int rows, int col, double* buf);

	//деструктор
	~Matrix();

	//выводит матрицу в консоль
	void Show();

	//get
	int getRows();
	int getCol();
	double** getM();

	//Multiply
	Matrix Mult(Matrix B);

	//транспонирование
	void Trans();

	//Convert
	double* conToM();
	double* conToM(bool flag);
	void conToN(double* buf);
	void conToN(double* buf, bool flag);

	//delete rubbish in U matrix
	void delRubb();

	//reverse Sigma
	void reverse();

	//left error
	double lError(Matrix m);
	//right error
	double rError(Matrix A, Matrix B);

	void vectorX(vector <double>& vec);
};

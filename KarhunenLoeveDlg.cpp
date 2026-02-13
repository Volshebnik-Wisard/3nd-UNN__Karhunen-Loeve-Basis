
// KarhunenLoeveDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "KarhunenLoeve.h"
#include "KarhunenLoeveDlg.h"
#include "afxdialogex.h"
#include "SVD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define Pi 3.1415926535



// Диалоговое окно CKarhunenLoeveDlg
CKarhunenLoeveDlg::CKarhunenLoeveDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SVD_DIALOG, pParent)
	/*
	, A1(1)
	, A2(2)
	, A3(3)
	, F1(0.01)
	, F2(0.03)
	, F3(0.05)
	, PHASE1(0)
	, PHASE2(0)
	, PHASE3(0)
	, N(1024)
	, FD(1)
	, P(100)
	, NUMBERV(1)
	, NUMBERU(1)
	*/

	, A1(10)
	, A2(9)
	, A3(5)
	, F1(2)
	, F2(3)
	, F3(4)
	, PHASE1(0)
	, PHASE2(1)
	, PHASE3(2)
	, N(1024)
	, FD(100)
	, P(130)
	, NUMBERV(1)
	, NUMBERU(1)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKarhunenLoeveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITA1, A1);
	DDX_Text(pDX, IDC_EDITA2, A2);
	DDX_Text(pDX, IDC_EDITA3, A3);
	DDX_Text(pDX, IDC_EDITF1, F1);
	DDX_Text(pDX, IDC_EDITF2, F2);
	DDX_Text(pDX, IDC_EDITF3, F3);
	DDX_Text(pDX, IDC_EDITALPHA1, PHASE1);
	DDX_Text(pDX, IDC_EDITALPHA2, PHASE2);
	DDX_Text(pDX, IDC_EDITALPHA3, PHASE3);
	DDX_Text(pDX, IDC_EDITN, N);
	DDX_Text(pDX, IDC_EDITFD, FD);
	DDX_Text(pDX, IDC_EDITP, P);
	DDX_Text(pDX, IDC_EDITNUMBERV, NUMBERV);
	DDX_Text(pDX, IDC_EDITNUMBERU, NUMBERU);
	DDX_Control(pDX, IDC_SPIN1, SpinV);
	DDX_Control(pDX, IDC_SPIN2, SpinU);
}

BEGIN_MESSAGE_MAP(CKarhunenLoeveDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDGENERATESIGN, &CKarhunenLoeveDlg::OnBnClickedGeneratesign)
	ON_BN_CLICKED(IDDRAWV, &CKarhunenLoeveDlg::OnBnClickedDrawv)
	ON_BN_CLICKED(IDDRAWU, &CKarhunenLoeveDlg::OnBnClickedDrawu)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CKarhunenLoeveDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CKarhunenLoeveDlg::OnDeltaposSpin2)
END_MESSAGE_MAP()


// Обработчики сообщений CKarhunenLoeveDlg

BOOL CKarhunenLoeveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Sign.Create(GetDlgItem(IDC_SIGN)->GetSafeHwnd());
	V.Create(GetDlgItem(IDC_V)->GetSafeHwnd());
	U.Create(GetDlgItem(IDC_U)->GetSafeHwnd());
	SingularValues.Create(GetDlgItem(IDC_SINGULAR)->GetSafeHwnd());

	SpinV.SetRange(0, P - 1);
	SpinV.SetPos(NUMBERV);
	SpinU.SetRange(0, P - 1);
	SpinU.SetPos(NUMBERU);

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CKarhunenLoeveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CKarhunenLoeveDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CKarhunenLoeveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKarhunenLoeveDlg::OnBnClickedGeneratesign()
{
	// TODO: добавьте свой код обработчика уведомлений
	Signal.clear();
	keys.clear();
	SIGMA.clear();

	srand(time(0));
	Sign.DrawW();
	V.DrawW();
	U.DrawW();
	SingularValues.DrawW();
	UpdateData(TRUE);

	CreateSignal();
	xmax = (N - 1) / FD,
		ymax = FindMax(Signal, N),
		ymin = FindMin(Signal, N);
	Sign.DrawOne(Signal, 0, xmax, ymin, ymax, keys);

	Autocorrelation();
	Matrix SigmaM(P, P);
	SigmaM.conToN(SigmaVEC, 1);
	for (int i = 0; i < P; i++)
	{
		SIGMA.push_back(SigmaVEC[i]);
	}
	xmax = P;
	ymax = FindMax(SIGMA, P),
		ymin = FindMin(SIGMA, P);
	SingularValues.DrawOneEllipse(SIGMA, 0, xmax, ymin, ymax, keys);
	/*
	Signal.clear();
	keys.clear();
	SIGMA.clear();
	*/
}


void CKarhunenLoeveDlg::OnBnClickedDrawv()
{
	// TODO: добавьте свой код обработчика уведомлений
	srand(time(0));
	V.DrawW();
	UpdateData(TRUE);

	GetVecV();
	xmax = P;
	ymax = FindMax(vvec, P),
		ymin = FindMin(vvec, P);
	V.DrawOne(vvec, 0, xmax, ymin, ymax, pvvec);

	vvec.clear();
	pvvec.clear();
	SpinV.SetPos(NUMBERV);
}


void CKarhunenLoeveDlg::OnBnClickedDrawu()
{
	// TODO: добавьте свой код обработчика уведомлений
	srand(time(0));
	U.DrawW();
	UpdateData(TRUE);

	GetVecU();
	xmax = P;
	ymax = FindMax(uvec, P),
		ymin = FindMin(uvec, P);
	U.DrawOne(uvec, 0, xmax, ymin, ymax, puvec);
	uvec.clear();
	puvec.clear();
	SpinU.SetPos(NUMBERU);
}

/*Ф-ция, вычисляющая предел графика*/
double CKarhunenLoeveDlg::FindMin(vector <double> data, int N)
{
	double min = data[0];
	for (int i = 0; i < N; i++)
	{
		if (min > data[i])
			min = data[i];
	}
	return min;
}
/*Ф-ция, вычисляющая предел графика*/
double CKarhunenLoeveDlg::FindMax(vector <double> data, int N)
{
	double max = data[0];
	for (int i = 0; i < N; i++)
	{
		if (max < data[i])
			max = data[i];
	}
	return max;
}

void CKarhunenLoeveDlg::CreateSignal()
{
	for (int i = 0; i < N; i++)
	{
		Signal.push_back(
			A1 * sin(2 * Pi * F1 * (i / FD) + PHASE1) + A2 * sin(2 * Pi * F2 * (i / FD) + PHASE2) + A3 * sin(2 * Pi * F3 * (i / FD) + PHASE3)
		);
		keys.push_back(i / FD);
	}
}

void CKarhunenLoeveDlg::Autocorrelation()
{
	rxx.resize(P);
	for (int m = 0; m < P; m++)
	{
		rxx[m] = 0;
		for (int n = 0; n < N - P - 1; n++)
		{
			rxx[m] += Signal[n + m] * Signal[n];
		}
		rxx[m] = rxx[m] / (N - m);
	}

	for (int i = 0; i < 2 * P; i++)
	{
		if (i < P)
			rnew.push_back(rxx[P - 1 - i]);
		if (i > P)
			rnew.push_back(rxx[i - P]);
	}

	MatrixR.resize(N, vector<double>(N));
	int index_i = 0;
	int index_j = 0;
	for (int i = 0; i < P; i++)
	{
		for (int j = 0; j < P; j++)
		{
			MatrixR[i][j] = rnew[P - 1 - j + index_i];
			MatrixA[index_j] = MatrixR[i][j];
			index_j++;
		}
		index_i++;
	}

	SVD(P, P, MatrixA, UVEC, VVEC, SigmaVEC);
}

void CKarhunenLoeveDlg::GetVecU()
{
	UpdateData(TRUE);
	CreateSignal();
	Autocorrelation();

	Matrix UM(P, P);
	UM.conToN(UVEC);

	double** u = UM.getM();
	for (int i = 0; i < P; i++)
	{
		uvec.push_back(u[i][(int)NUMBERU]);
		puvec.push_back(i);
	}
}

void CKarhunenLoeveDlg::GetVecV()
{
	UpdateData(TRUE);
	CreateSignal();
	Autocorrelation();

	Matrix VM(P, P);
	VM.conToN(VVEC);

	double** v = VM.getM();
	for (int i = 0; i < P; i++)
	{
		vvec.push_back(v[i][(int)NUMBERV]);
		pvvec.push_back(i);
	}
}

//Перевод из одномерного массива в двумерный
double** CKarhunenLoeveDlg::conToN(double* buf)
{
	double** A = new double* [rows];
	for (int i = 0; i < rows; i++)
	{
		A[i] = new double[col];
		for (int j = 0; j < col; j++)
		{
			int id = i * col + j;
			A[i][j] = buf[id];
		}
	}
	//M = A;
	return A;
}

//Перевод из одномерного массива в двумерный для сингулярных
double** CKarhunenLoeveDlg::conToN(double* buf, bool flag)
{
	double** A = new double* [rows];
	for (int i = 0; i < rows; i++) {
		A[i] = new double[col];
		for (int j = 0; j < col; j++) {
			if (i == j)
				A[i][j] = 1 / buf[i];
			else
				A[i][j] = 0;
		}
	}
	return A;
}

//Умножение матриц
double** CKarhunenLoeveDlg::Mult(double** A, double** B, int rowsB, int colB, int rows, int cols)
{

	double** M = new double* [rows];
	for (int i = 0; i < rows; i++)
	{
		M[i] = new double[colB];
	}

	//m - row, n - col
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < colB; j++)
		{
			M[i][j] = 0;
			for (int k = 0; k < rowsB; k++)
			{
				if (cols - 1 >= k)
					M[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return M;
}

void CKarhunenLoeveDlg::Trans(double** M)
{
	double** A = new double* [col];


	for (int i = 0; i < col; i++)
	{
		A[i] = new double[rows];
		for (int j = 0; j < rows; j++)
			A[i][j] = M[j][i];
	}
	int buf = col;
	col = rows;
	rows = buf;
	M = A;
}

Matrix::Matrix(int rows, int col)
{
	this->rows = rows;
	this->col = col;

	M = new double* [rows];

	for (int i = 0; i < rows; i++) {
		M[i] = new double[col];
		for (int j = 0; j < col; j++) {
			M[i][j] = 1 + (double)rand() / RAND_MAX;
		}
	}
}

Matrix::Matrix()
{

}

Matrix::Matrix(int rows, int col, double* buf)
{
	this->rows = rows;
	this->col = col;
	M = new double* [rows];
	//перегон матрицы из одномерного массива
	for (int i = 0; i < rows; i++) {
		M[i] = new double[col];
		for (int j = 0; j < col; j++) {
			int id = i * col + j;
			M[i][j] = buf[id];
		}
	}
}

//деструктор
Matrix::~Matrix()
{

}

//выводит матрицу в консоль
void Matrix::Show()
{
	for (int i = 0; i < rows; i++) {
		cout << "| ";
		for (int j = 0; j < col; j++) {
			cout << M[i][j] << " ";
		}
		cout << '|' << endl;
	}
}

//get
int Matrix::getRows()
{
	return rows;
}

int Matrix::getCol()
{
	return col;
}

double** Matrix::getM()
{
	return M;
}

//Multiply
Matrix Matrix::Mult(Matrix B)
{
	int rowsB = B.getRows();
	int colB = B.getCol();

	if (col > rowsB) {
		cout << "Ошибка перемножения!" << endl;
		return *this;
	}

	Matrix res;
	res.col = colB;
	res.rows = rows;


	res.M = new double* [rows];
	for (int i = 0; i < rows; i++) {
		res.M[i] = new double[colB];
	}

	//m - row, n - col
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < colB; j++) {
			res.M[i][j] = 0;
			for (int k = 0; k < rowsB; k++) {
				if (col - 1 >= k)
					res.M[i][j] += M[i][k] * B.M[k][j];
			}
		}
	}
	return res;
}

//транспонирование
void Matrix::Trans()
{
	double** A = new double* [col];


	for (int i = 0; i < col; i++) {
		A[i] = new double[rows];
		for (int j = 0; j < rows; j++)
			A[i][j] = M[j][i];
	}
	int buf = col;
	col = rows;
	rows = buf;
	M = A;
}

//переводит из двойного массива в одинарный
double* Matrix::conToM() {
	double* res = new double[rows * col];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < col; j++) {
			int id = i * col + j;
			res[id] = M[i][j];
		}
	return res;
}

double* Matrix::conToM(bool flag) {
	double* res = new double[rows * col];
	return res;
}

//переводит из одинарного в двойной
void Matrix::conToN(double* buf) {

	double** A = new double* [rows];
	for (int i = 0; i < rows; i++) {
		A[i] = new double[col];
		for (int j = 0; j < col; j++) {
			int id = i * col + j;
			A[i][j] = buf[id];
		}
	}
	M = A;
}

//переводит из одинарного в двойной диоганальную матрицу
void Matrix::conToN(double* buf, bool flag) {

	double** A = new double* [rows];
	for (int i = 0; i < rows; i++) {
		A[i] = new double[col];
		for (int j = 0; j < col; j++) {
			if (i == j)
				A[i][j] = buf[i];
			else
				A[i][j] = 0;
		}
	}
	M = A;
}

//удаляет ненужные данные из матрицы U
void Matrix::delRubb() {
	int rows = this->rows;
	int col = this->col;
	this->col = rows;

	double* res = new double(rows * rows);

	//заполняется матрица V, как единичная
	for (int i = 0; i < rows; i++) {
		int in = i * rows;
		for (int j = 0; j < rows; j++)
			res[in + j] = this->M[i][j];
	}

	this->conToN(res);
}

//reverse Sigma
void Matrix::reverse() {
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < rows; j++)
			if (i == j)
				this->M[i][j] = 1 / this->M[i][j];
	}
}

//left error
double Matrix::lError(Matrix Xres)
{
	double res = 0;
	for (int i = 0; i < this->rows; i++)
		res += abs(this->M[i][0] - Xres.M[i][0]);
	return res;
}
//right error
double Matrix::rError(Matrix A, Matrix B)
{
	double res = 0;

	Matrix buf = A.Mult(*this);
	for (int i = 0; i < B.rows; i++)
		res += abs(B.M[i] - buf.M[i]);
	return res;
}

void Matrix::vectorX(vector <double>& y)
{
	for (int i = 0; i < this->rows; i++)
	{
		y.push_back(this->M[i][0]);
	}
}



void CKarhunenLoeveDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{

	NUMBERV = SpinV.GetPos() - 65536;
	UpdateData(FALSE);
	srand(time(0));
	V.DrawW();
	UpdateData(TRUE);
	GetVecV();
	xmax = P;
	ymax = FindMax(vvec, P),
		ymin = FindMin(vvec, P);
	V.DrawOne(vvec, 0, xmax, ymin, ymax, pvvec);
	vvec.clear();
	pvvec.clear();
}


void CKarhunenLoeveDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult)
{

	NUMBERU = SpinU.GetPos() - 65536;
	UpdateData(FALSE);
	srand(time(0));
	U.DrawW();
	UpdateData(TRUE);
	GetVecU();
	xmax = P;
	ymax = FindMax(uvec, P),
		ymin = FindMin(uvec, P);
	U.DrawOne(uvec, 0, xmax, ymin, ymax, puvec);
	uvec.clear();
	puvec.clear();

}




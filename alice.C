#include <string>
#include <iostream>
#include <fstream>
#include <vector>

// Eigen
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace std;

int main()
{
	cout<<"Hello"<<endl;

	// stage 1 : import correspondences
	ifstream fread("featurePoints.txt");

	if(fread.fail()) 
	{
		cout<<"Data File dose not exist."<<endl;
		return 0;
	}

	double x_R = 0;
	double y_R = 0;
	double x_C = 0;
	double y_C = 0;

	vector<double> xs_R;
	vector<double> ys_R;
	vector<double> xs_C;
	vector<double> ys_C;

	while(!fread.eof())
	{
		fread>>x_R>>y_R>>x_C>>y_C;

		if(fread.eof()) break;

		xs_R.push_back(x_R);
		ys_R.push_back(y_R);
		xs_C.push_back(x_C);
		ys_C.push_back(y_C);

		//cout<<x_R<<"	"<<y_R<<"	"<<x_C<<"	"<<y_C<<endl;
	}
	fread.close();

	/*
	for(int i=0; i<xs_R.size(); i++)
	{
		cout<<xs_R[i]<<"	"<<ys_R[i]<<"	"<<xs_C[i]<<"	"<<ys_C[i]<<endl;	
	}
	*/


	// stage 2 : make matrix A
	int len = xs_R.size();
	Eigen::MatrixXd A(len, 9);
	for(int i=0; i<len; i++)
	{
		A(i,0) = xs_C[i] * xs_R[i];
		A(i,1) = xs_C[i] * ys_R[i];
		A(i,2) = xs_C[i];

		A(i,3) = ys_C[i] * xs_R[i];
		A(i,4) = ys_C[i] * ys_R[i];
		A(i,5) = ys_C[i];
	
		A(i,6) = xs_R[i];
		A(i,7) = ys_R[i];
		A(i,8) = 1.;
	}
	//cout<<"A"<<endl;
	//cout<<A<<endl;

	// stage 3 : LMS
	Eigen::SelfAdjointEigenSolver < Eigen::Matrix<double, 9, 9> > eigen_sovler (A.transpose()*A);
	cout<<"Eigen Values : "<<endl;
	cout<<eigen_sovler.eigenvalues().transpose()<<endl;	
	cout<<"Eigen Vectors : "<<endl;
	cout<<eigen_sovler.eigenvectors()<<endl;	

	
	Eigen::MatrixXd F(3, 3);

	F(0,0) = eigen_sovler.eigenvectors()(0,0);
	F(0,1) = eigen_sovler.eigenvectors()(1,0);
	F(0,2) = eigen_sovler.eigenvectors()(2,0);

	F(1,0) = eigen_sovler.eigenvectors()(3,0);
	F(1,1) = eigen_sovler.eigenvectors()(4,0);
	F(1,2) = eigen_sovler.eigenvectors()(5,0);

	F(2,0) = eigen_sovler.eigenvectors()(6,0);
	F(2,1) = eigen_sovler.eigenvectors()(7,0);
	F(2,2) = eigen_sovler.eigenvectors()(8,0);

	cout<<"Fundamental Matrix:"<<endl;
	cout<<F<<endl;
	return 0;
}

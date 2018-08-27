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

	// stage 1 : import data
	ifstream fread("data.txt");

	if(fread.fail()) 
	{
		cout<<"Data File dose not exist."<<endl;
		return 0;
	}

	double x=0;
	double y=0;
	vector<double> xs;
	vector<double> ys;

	while(!fread.eof())
	{
		fread>>x>>y;

		if(fread.eof()) break;

		xs.push_back(x);
		ys.push_back(y);
	}

	// stage 2 : make matrix
	int len = xs.size();

	Eigen::MatrixXd A(len, 3);
	for(int i=0; i<len; i++)
	{
		A(i,0) = xs[i];	
		A(i,1) = 1.;	
		A(i,2) = -1.*ys[i];	
	}
	//cout<<"A"<<endl;
	//cout<<A<<endl;

	// stage 3 : LMS
	Eigen::SelfAdjointEigenSolver < Eigen::Matrix<double, 3, 3> > eigen_sovler (A.transpose()*A);
	cout<<"Eigen Values : "<<endl;
	cout<<eigen_sovler.eigenvalues().transpose()<<endl;	
	cout<<"Eigen Vectors : "<<endl;
	cout<<eigen_sovler.eigenvectors()<<endl;	

	double v[3];
	v[0] = eigen_sovler.eigenvectors()(0,0);
	v[1] = eigen_sovler.eigenvectors()(1,0);
	v[2] = eigen_sovler.eigenvectors()(2,0);

	v[0] /= v[2];
	v[1] /= v[2];
	v[2] /= v[2];

	cout<<v[0]<<"	"<<v[1]<<"	"<<v[2]<<"	"<<endl;

	



	return 0;
}

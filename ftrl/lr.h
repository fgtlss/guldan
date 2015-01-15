#ifndef LR_H_
#define LR_H_

#include <string>
#include <map>
#include <math.h>
#include <time.h>
#include <vector>
#include <set>
#include <fstream>

#include "../util/sparse_vector.h"
#include "../util/string_util.h"
#include "../util/file_control.h"
#include "../util/amath.h"
#include "../util/random.h"

//FTRL的类


class logistic_regression{
 private:
  
  //基本与论文的名称相对应
  double alpha;
  double beta;
  double L1;
  double L2;

  Sparse_Vector z;
  Sparse_Vector n;
  Sparse_Vector sigma;
  Sparse_Vector g;
  Sparse_Vector w;

  Sparse_Vector bm;

  int iter;
  int verbose;

  long pos ;
  long neg ;

  sp_iter it;
  bool update;
  long no_train;

 public:

  //构造函数
  logistic_regression(double alpha1,double beta1,double l1,double l2,	\
		      int iter1,int vb):					\
  alpha(alpha1),beta(beta1),L1(l1),L2(l2),z(),n(),sigma(),g(),w(),iter(iter1),verbose(vb),bm(){
  }

  void print(){
    std::cout<<"lr"<<std::endl;
  }

  void fit(std::string train_dir,double stop);

  void one_round(int label,Sparse_Vector& x);

  void update_w(Sparse_Vector& x);
  
  void update_other(Sparse_Vector& x,double p,int label);

  double predict(std::string test_dir,std::string out_dir);

};

void logistic_regression::update_w(Sparse_Vector& x){
  //----------------------更新w---------------------------------
  if (update==false){
    update = true;
    no_train ++ ;
    return ;
  }
  
  x.restart();
  while(x.has_next()) {
    it = x.next();
    long indx = it->first;
    //double xvalue = it->second;
    double zvalue = z.get_value(indx);

    if(abs(zvalue)<=L1){
      w.set_value(indx,0);
    }
    else{
      double a1 = -zvalue + sgn(zvalue)*L1;
      double a2 = (beta+sqrt(n.get_value(indx)))/alpha + L2;

      w.set_value(indx,a1/a2);
    }
  }

}

void logistic_regression::update_other(Sparse_Vector& x,double p,int label){
  //-----------------------更新g,sigma,z,n----------------------------
  g = x * (p-label);

  //只有负的进行滤特征
  //if(label == 0)
  //g.poisson_filter();
  //g.bloom_filter(bm,4);

  g.restart();
  while(g.has_next()){
    it = g.next();
    long indx = it->first;
    double value = it->second;

    double nvalue = n.get_value(indx);
    
    double ssigma = (sqrt(nvalue + value*value)-sqrt(nvalue))/alpha;

    double zvalue = z.get_value(indx);
    z.set_value(indx,zvalue + value-ssigma * w.get_value(indx));

    n.set_value(indx,nvalue + value*value);

  }

  if(g.size()==0)
    update = false;

}

void logistic_regression::fit(std::string train_dir,double stop){
  srand(time(0));
  File_Control fc(train_dir);

  stop *= (1000*1000);
  long pos = 0;
  long neg = 0;
  no_train = 0;

  update = true;
  
  //训练第几遍文件
  int indx = 0;
  for(int i=0;i<iter;i++){
    std::cout<<"训练第"<<i<<"遍文件"<<std::endl;
    while(fc.has_next() && indx <= stop){
      string next = fc.next();
      if(next.size()>3){
	
	//subsampling
	double ran = ran_uniform();
	std::pair<int,Sparse_Vector> result = sparse_vector_form(next);
	
	if(result.first==1 || (ran<1)){
	  if (result.first==1)
	    pos ++ ;
	  else if (result.first==0)
	    neg ++ ;
	  
	  one_round(result.first,result.second);
	  
	}
	indx ++ ;
	if (indx%(verbose*1000)==0)
	  std::cout<<"indx "<<std::setw(5)<<(double)indx/(1000*1000)<<"M |w  "<<w.size()<<" |z  "<<z.size() \
		   <<" |g "<<g.size()<<" |no_train "<<no_train<<" |n "<<n.size() \
		   <<" |pos "<<pos<<" |neg "<<neg<<std::endl;
	
      }
    }

    if(indx>=stop){
      indx = 0;
    }
    indx = 0;
    
    fc.restart();
  }

}

void logistic_regression::one_round(int label,Sparse_Vector& x) {

  update_w(x);
  //-----------------------预测p--------------------------------
  //x.poisson_filter();
  double p = w.dot_sum(x);
  
  update_other(x,p,label);

}


double logistic_regression::predict(std::string test_dir,std::string out_dir){
  File_Control fc(test_dir);
  fc.restart();

  long total = 0;
  std::ofstream out(out_dir.c_str());
  
  while(fc.has_next() && total <= 100000){
    string next = fc.next();
    if(next.size()>3){
      std::pair<int,Sparse_Vector> result = sparse_vector_form(next);
      int label = result.first;
      Sparse_Vector x = result.second;

      int plabel = -1;
      double ds = w.dot_sum(x);
      double p = sigmoid(ds);
      out<<label<<" "<<p<<std::endl;

      total ++ ;
    }
  }

  out.close();
  
  return 0;
}


#endif

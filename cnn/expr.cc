#include "cnn/expr.h"

#include <initializer_list>

#include "cnn/nodes.h"
#include "cnn/conv.h"

namespace cnn { namespace expr {

Expression input(ComputationGraph& g, real s) { return Expression(&g, g.add_input(s)); }
Expression input(ComputationGraph& g, const real *ps) { return Expression(&g, g.add_input(ps)); }
Expression input(ComputationGraph& g, const Dim& d, const std::vector<float>* pdata) { return Expression(&g, g.add_input(d, pdata)); }
Expression parameter(ComputationGraph& g, Parameters* p) { return Expression(&g, g.add_parameters(p)); }
Expression lookup(ComputationGraph& g, LookupParameters* p, unsigned index) { return Expression(&g, g.add_lookup(p, index)); }
Expression lookup(ComputationGraph& g, LookupParameters* p, const unsigned* pindex) { return Expression(&g, g.add_lookup(p, pindex)); }
Expression const_lookup(ComputationGraph& g, LookupParameters* p, unsigned index) { return Expression(&g, g.add_const_lookup(p, index)); }
Expression const_lookup(ComputationGraph& g, LookupParameters* p, const unsigned* pindex) { return Expression(&g, g.add_const_lookup(p, pindex)); }

Expression operator-(const Expression& x) { return Expression(x.pg, x.pg->add_function<Negate>({x.i})); }
Expression operator+(const Expression& x, const Expression& y) { return Expression(x.pg, x.pg->add_function<Sum>({x.i, y.i})); }
Expression operator-(const Expression& x, const Expression& y) { return x+(-y); }
Expression operator-(real x, const Expression& y) { return Expression(y.pg, y.pg->add_function<ConstantMinusX>({y.i}, x)); }
Expression operator*(const Expression& x, const Expression& y) { return Expression(x.pg, x.pg->add_function<MatrixMultiply>({x.i, y.i})); }
Expression cdiv(const Expression& x, const Expression& y) { return Expression(x.pg, x.pg->add_function<CwiseQuotient>({x.i, y.i})); }

Expression tanh(const Expression& x) { return Expression(x.pg, x.pg->add_function<Tanh>({x.i})); }
Expression log(const Expression& x) { return Expression(x.pg, x.pg->add_function<Log>({x.i})); }
Expression exp(const Expression& x) { return Expression(x.pg, x.pg->add_function<Exp>({x.i})); }
Expression logistic(const Expression& x) { return Expression(x.pg, x.pg->add_function<LogisticSigmoid>({x.i})); }
Expression rectify(const Expression& x) { return Expression(x.pg, x.pg->add_function<Rectify>({x.i})); }
Expression log_softmax(const Expression& x) { return Expression(x.pg, x.pg->add_function<LogSoftmax>({x.i})); }
Expression log_softmax(const Expression& x, const std::vector<unsigned>& d) { return Expression(x.pg, x.pg->add_function<RestrictedLogSoftmax>({x.i}, d)); }
Expression softmax(const Expression& x) { return Expression(x.pg, x.pg->add_function<Softmax>({x.i})); }
Expression softsign(const Expression& x) { return Expression(x.pg, x.pg->add_function<SoftSign>({x.i})); }
Expression noise(const Expression& x, real stddev) { return Expression(x.pg, x.pg->add_function<GaussianNoise>({x.i}, stddev)); }
Expression dropout(const Expression& x, real p) { return Expression(x.pg, x.pg->add_function<Dropout>({x.i}, p)); }

Expression reshape(const Expression& x, const Dim& d) { return Expression(x.pg, x.pg->add_function<Reshape>({x.i}, d)); }
Expression transpose(const Expression& x) { return Expression(x.pg, x.pg->add_function<Transpose>({x.i})); }

Expression affine_transform(const std::initializer_list<Expression>& xs) {
  ComputationGraph *pg = xs.begin()->pg;
  std::vector<VariableIndex> xis(xs.size());
  int i = 0;
  for (auto& e : xs) xis[i++] = e.i;
  return Expression(pg, pg->add_function<AffineTransform>(xis));
}
Expression cwise_multiply(const Expression& x, const Expression& y) {return Expression(x.pg, x.pg->add_function<CwiseMultiply>({x.i, y.i}));}

Expression dot_product(const Expression& x, const Expression& y) { return Expression(x.pg, x.pg->add_function<DotProduct>({x.i, y.i})); }
Expression squared_distance(const Expression& x, const Expression& y) { return Expression(x.pg, x.pg->add_function<SquaredEuclideanDistance>({x.i, y.i})); }
Expression huber_distance(const Expression& x, const Expression& y, real c) { return Expression(x.pg, x.pg->add_function<HuberDistance>({x.i, y.i}, c)); }
Expression l1_distance(const Expression& x, const Expression& y) { return Expression(x.pg, x.pg->add_function<L1Distance>({x.i, y.i})); }
Expression binary_log_loss(const Expression& x, const Expression& y) { return Expression(x.pg, x.pg->add_function<BinaryLogLoss>({x.i,y.i})); }
Expression pairwise_rank_loss(const Expression& x, const Expression& y, real m) { return Expression(x.pg, x.pg->add_function<PairwiseRankLoss>({x.i, y.i}, m)); }

Expression conv1d_narrow(const Expression& x, const Expression& f) { return Expression(x.pg, x.pg->add_function<Conv1DNarrow>({x.i, f.i})); }
//Expression conv1d_wide(const Expression& x, const Expression& f) { return Expression(x.pg, x.pg->add_function<Conv1DWide>({x.i, f.i})); }
Expression kmax_pooling(const Expression& x, unsigned k) { return Expression(x.pg, x.pg->add_function<KMaxPooling>({x.i}, k)); }

Expression pick(const Expression& x, unsigned v) { return Expression(x.pg, x.pg->add_function<PickElement>({x.i}, v)); }
Expression pick(const Expression& x, unsigned* pv) { return Expression(x.pg, x.pg->add_function<PickElement>({x.i}, pv)); }
Expression pickrange(const Expression& x, unsigned v, unsigned u) { return Expression(x.pg, x.pg->add_function<PickRange>({x.i}, v, u)); }

Expression pickneglogsoftmax(const Expression& x, unsigned v) { return Expression(x.pg, x.pg->add_function<PickNegLogSoftmax>({x.i}, v)); }

Expression sum_cols(const Expression& x) { return Expression(x.pg, x.pg->add_function<SumColumns>({x.i})); }

Expression kmh_ngram(const Expression& x, unsigned n) { return Expression(x.pg, x.pg->add_function<KMHNGram>({x.i}, n)); }

} }
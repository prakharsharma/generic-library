namespace kick_ass_lib {

struct nil {};

template<class HT, class TT>
struct cons {
	HT head;
	TT tail;

	template<class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		cons (const HT& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
				const T6& t6, const T7& t7, const T8& t8) : head(t1),
		tail(t2, t3, t4, t5, t6, t7, t8, nil()) {}

	template<class HT2, class TT2>
		cons (const cons<HT2, TT2>& u) : head(u.head), tail(u.tail) {}
};
template<class HT> /*partial specialization for cons*/
struct cons<HT, nil> {
	HT head;

	cons (const HT& t1, const nil&, const nil&, const nil&, const nil&, const nil&,
			const nil&, const nil&) : head(t1) {}

	template<class HT2>
		cons (const cons<HT2, nil>& u) : head(u.head) {}
};

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
struct tuple_to_cons {
	typedef cons<T1, typename tuple_to_cons<T2, T3, T4, T5, T6, T7, T8, nil>::U> U;
};
template<class T1> /*partial specialization for tuple_to_cons*/
struct tuple_to_cons<T1, nil, nil, nil, nil, nil, nil, nil> {
	typedef cons<T1, nil> U;
};

template<class T>
inline const T& wrap () {
	return T();
}

template<class T1, class T2=nil, class T3=nil, class T4=nil, class T5=nil, class T6=nil,
       class T7=nil, class T8=nil>
struct tuple : public tuple_to_cons<T1, T2, T3, T4, T5, T6, T7, T8>::U {
	tuple (const T1& t1=wrap<T1>(), const T2& t2=wrap<T2>(), const T3& t3=wrap<T3>(),
			const T4& t4=wrap<T4>(), const T5& t5=wrap<T5>(),
			const T6& t6=wrap<T6>(), const T7& t7=wrap<T7>(),
			const T8& t8=wrap<T8>()) : 
		tuple_to_cons<T1, T2, T3, T4, T5, T6, T7, T8>::U (t1, t2, t3, t4, t5, t6,
				t7, t8) {}

	template<class U1, class U2>
		tuple (const cons<U1, U2>& p) : tuple_to_cons<T1, T2, T3, T4, T5, T6, T7, T8>::U (p) {}
};

template<int N>
struct nth {
	template<class HT, class TT>
		static void* get (cons<HT, TT>& t) {
			return nth<N-1>::get(t.tail);
		}
};
template<> /*full specialization for nth*/
struct nth<1> {
	template<class HT, class TT>
		static void* get(cons<HT, TT>& t) {
			return &t.head;
		}
};

template<int N, class T> struct nth_type;

template<int N, class HT, class TT>
struct nth_type<N, cons<HT, TT> > {
	typedef typename nth_type<N-1, TT>::U U;
};
template<class HT, class TT> /*partial specialization for nth_type*/
struct nth_type<1, cons<HT, TT> > {
	typedef HT U;
};

template<int N, class HT, class TT>
typename nth_type<N, cons<HT, TT> >::U& get(cons<HT, TT>& c) {
	typedef typename nth_type<N, cons<HT, TT> >::U return_type;
	return *static_cast<return_type*>(nth<N>::get(c));
}

}

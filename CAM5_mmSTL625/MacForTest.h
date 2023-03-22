#ifndef MAC_FOR_TEST_H
#define MAC_FOR_TEST_H

#ifdef FOR_NO_VERTEX_NORMAL
#undef FOR_VERTEX_NORMAL
#endif // !FOR_NO_VERTEX_NORMAL

#ifdef FOR_VERTEX_NORMAL
#undef FOR_NO_VERTEX_NORMAL
#endif // FOR_VERTEX_NORMAL

#define POLYLINE_DISPLAY(L)									 \
if(L) {														\
	L->PolylineCheck();										\
	int n_##L = 0;											\
	for (int i = 1; i <= (L->DNum); i++)					\
		n_##L += (L->ENum[i] - L->SNum[i] + 1);				\
	for (size_t i = L->SNum[1]; i < n_##L; i++)				\
	{														\
		PNT3D p, q;											\
		memcpy(p, &L->PTrail[i], sizeof(PNT3D));			\
		memcpy(q, &L->PTrail[i + 1], sizeof(PNT3D));	\
		AddLin(p, q);										\
	}														\
	Redraw();												\
}

#define POLYLINE_DESTROY(L) \
if(L) {						\
	L->Destroy();			\
	free(L);				\
	L=nullptr;				\
}

#endif // !MAC_FOR_TEST_H
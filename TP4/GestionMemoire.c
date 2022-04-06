/*
 * TP4 - GestionMemoire.c
 * 
 * École Polytechnique de Montréal, Hiver 2022
 * Maxence Lefebvre, 2085581
 * Huy-Don Phan, 2074318
 * Section de laboratoire : 02 (B2)
 * 
 */

#include "./libs/lib.h"
unsigned int calculerNumeroDePage(unsigned long adresse) {
    return adresse / TAILLE_PAGE_CADRE;
}

unsigned long calculerDeplacementDansLaPage(unsigned long adresse) {
    return adresse % TAILLE_PAGE_CADRE;
}

unsigned long calculerAdresseComplete(unsigned int numeroDePage, unsigned long deplacementDansLaPage) {
    return numeroDePage * TAILLE_PAGE_CADRE + deplacementDansLaPage;
}

void rechercherTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	int numeroDePageReq = calculerNumeroDePage(req->adresseVirtuelle);
	for (int i = 0; i < TAILLE_TLB; i++) {
		if (mem->tlb->numeroPage[i] == numeroDePageReq && mem->tlb->entreeValide[i]) {
			req->adressePhysique = calculerAdresseComplete(mem->tlb->numeroCadre[i], calculerDeplacementDansLaPage(req->adresseVirtuelle));
			req->estDansTLB = 1;
			mem->tlb->dernierAcces[i] = req->date;
			return;
		}
	}
	req->adressePhysique = 0;
	req->estDansTLB = 0;
}

void rechercherTableDesPages(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	int numeroDePage = calculerNumeroDePage(req->adresseVirtuelle);
	if (mem->tp->entreeValide[numeroDePage]) {
		req->adressePhysique = calculerAdresseComplete(mem->tp->numeroCadre[numeroDePage], calculerDeplacementDansLaPage(req->adresseVirtuelle));
		req->estDansTablePages = 1;
	} else {
		req->adressePhysique = 0;
		req->estDansTablePages = 0;
	}
}

void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	int numeroDePage = calculerNumeroDePage(req->adresseVirtuelle);
	for (int i = 0; i < TAILLE_MEMOIRE; i++) {
		if (mem->memoire->utilisee[i] == 0) {
			req->adressePhysique = calculerAdresseComplete(mem->tlb->numeroCadre[i], calculerDeplacementDansLaPage(req->adresseVirtuelle));
			mem->memoire->numeroPage[i] = numeroDePage;
			mem->memoire->dateCreation[i] = req->date;
			mem->memoire->dernierAcces[i] = req->date;
			mem->memoire->utilisee[i] = 1;
		}
	}
}

void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
}

// NE PAS MODIFIER
int main() {
    evaluate(
		&calculerNumeroDePage, 
		&calculerDeplacementDansLaPage, 
		&calculerAdresseComplete, 
        &rechercherTLB, 
		&rechercherTableDesPages,
		&mettreAJourTLB,
		&ajouterDansMemoire
    );
    return 0;
}

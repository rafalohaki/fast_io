#include<fast_io.h>
#include<fast_io_i18n.h>
#include<fast_io_device.h>

int main()
{
	fast_io::obuf_file obf("lc_time.txt");
	constexpr char8_t const* files[]{u8"aa_DJ",u8"aa_ER",u8"aa_ER_saaho",u8"aa_ET",u8"af_ZA",u8"agr_PE",u8"ak_GH",u8"am_ET",u8"anp_IN",u8"an_ES",u8"ar_AE",u8"ar_BH",u8"ar_DZ",u8"ar_EG",u8"ar_IN",u8"ar_IQ",u8"ar_JO",u8"ar_KW",u8"ar_LB",u8"ar_LY",u8"ar_MA",u8"ar_OM",u8"ar_QA",u8"ar_SA",u8"ar_SD",u8"ar_SS",u8"ar_SY",u8"ar_TN",u8"ar_YE",u8"ast_ES",u8"as_IN",u8"ayc_PE",u8"az_AZ",u8"az_IR",u8"bem_ZM",u8"ber_DZ",u8"ber_MA",u8"be_BY",u8"be_BY_latin",u8"bg_BG",u8"bhb_IN",u8"bho_IN",u8"bho_NP",u8"bi_VU",u8"bn_BD",u8"bn_IN",u8"bo_CN",u8"bo_IN",u8"brx_IN",u8"br_FR",u8"br_FR_euro",u8"bs_BA",u8"byn_ER",u8"C",u8"ca_AD",u8"ca_ES",u8"ca_ES_euro",u8"ca_ES_valencia",u8"ca_FR",u8"ca_IT",u8"ce_RU",u8"chr_US",u8"ckb_IQ",u8"cmn_TW",u8"crh_UA",u8"csb_PL",u8"cs_CZ",u8"cv_RU",u8"cy_GB",u8"da_DK",u8"de_AT",u8"de_AT_euro",u8"de_BE",u8"de_BE_euro",u8"de_CH",u8"de_DE",u8"de_DE_euro",u8"de_IT",u8"de_LI",u8"de_LU",u8"de_LU_euro",u8"doi_IN",u8"dsb_DE",u8"dv_MV",u8"dz_BT",u8"el_CY",u8"el_GR",u8"el_GR_euro",u8"en_AG",u8"en_AU",u8"en_BW",u8"en_CA",u8"en_DK",u8"en_GB",u8"en_HK",u8"en_IE",u8"en_IE_euro",u8"en_IL",u8"en_IN",u8"en_NG",u8"en_NZ",u8"en_PH",u8"en_SC",u8"en_SG",u8"en_US",u8"en_ZA",u8"en_ZM",u8"en_ZW",u8"eo",u8"es_AR",u8"es_BO",u8"es_CL",u8"es_CO",u8"es_CR",u8"es_CU",u8"es_DO",u8"es_EC",u8"es_ES",u8"es_ES_euro",u8"es_GT",u8"es_HN",u8"es_MX",u8"es_NI",u8"es_PA",u8"es_PE",u8"es_PR",u8"es_PY",u8"es_SV",u8"es_US",u8"es_UY",u8"es_VE",u8"et_EE",u8"eu_ES",u8"eu_ES_euro",u8"fa_IR",u8"ff_SN",u8"fil_PH",u8"fi_FI",u8"fi_FI_euro",u8"fo_FO",u8"fr_BE",u8"fr_BE_euro",u8"fr_CA",u8"fr_CH",u8"fr_FR",u8"fr_FR_euro",u8"fr_LU",u8"fr_LU_euro",u8"fur_IT",u8"fy_DE",u8"fy_NL",u8"ga_IE",u8"ga_IE_euro",u8"gd_GB",u8"gez_ER",u8"gez_ER_abegede",u8"gez_ET",u8"gez_ET_abegede",u8"gl_ES",u8"gl_ES_euro",u8"gu_IN",u8"gv_GB",u8"hak_TW",u8"ha_NG",u8"he_IL",u8"hif_FJ",u8"hi_IN",u8"hne_IN",u8"hr_HR",u8"hsb_DE",u8"ht_HT",u8"hu_HU",u8"hy_AM",u8"i18n",u8"ia_FR",u8"id_ID",u8"ig_NG",u8"ik_CA",u8"is_IS",u8"it_CH",u8"it_IT",u8"it_IT_euro",u8"iu_CA",u8"ja_JP",u8"kab_DZ",u8"ka_GE",u8"kk_KZ",u8"kl_GL",u8"km_KH",u8"kn_IN",u8"kok_IN",u8"ko_KR",u8"ks_IN",u8"ks_IN_devanagari",u8"ku_TR",u8"kw_GB",u8"ky_KG",u8"lb_LU",u8"lg_UG",u8"lij_IT",u8"li_BE",u8"li_NL",u8"ln_CD",u8"lo_LA",u8"lt_LT",u8"lv_LV",u8"lzh_TW",u8"mag_IN",u8"mai_IN",u8"mai_NP",u8"mfe_MU",u8"mg_MG",u8"mhr_RU",u8"miq_NI",u8"mi_NZ",u8"mjw_IN",u8"mk_MK",u8"ml_IN",u8"mni_IN",u8"mnw_MM",u8"mn_MN",u8"mr_IN",u8"ms_MY",u8"mt_MT",u8"my_MM",u8"nan_TW",u8"nan_TW_latin",u8"nb_NO",u8"nds_DE",u8"nds_NL",u8"ne_NP",u8"nhn_MX",u8"niu_NU",u8"niu_NZ",u8"nl_AW",u8"nl_BE",u8"nl_BE_euro",u8"nl_NL",u8"nl_NL_euro",u8"nn_NO",u8"nr_ZA",u8"nso_ZA",u8"oc_FR",u8"om_ET",u8"om_KE",u8"or_IN",u8"os_RU",u8"pap_AW",u8"pap_CW",u8"pa_IN",u8"pa_PK",u8"pl_PL",u8"POSIX",u8"ps_AF",u8"pt_BR",u8"pt_PT",u8"pt_PT_euro",u8"quz_PE",u8"raj_IN",u8"ro_RO",u8"ru_RU",u8"ru_UA",u8"rw_RW",u8"sah_RU",u8"sat_IN",u8"sa_IN",u8"sc_IT",u8"sd_IN",u8"sd_IN_devanagari",u8"se_NO",u8"sgs_LT",u8"shn_MM",u8"shs_CA",u8"sid_ET",u8"si_LK",u8"sk_SK",u8"sl_SI",u8"sm_WS",u8"so_DJ",u8"so_ET",u8"so_KE",u8"so_SO",u8"sq_AL",u8"sq_MK",u8"sr_ME",u8"sr_RS",u8"sr_RS_latin",u8"ss_ZA",u8"st_ZA",u8"sv_FI",u8"sv_FI_euro",u8"sv_SE",u8"sw_KE",u8"sw_TZ",u8"szl_PL",u8"ta_IN",u8"ta_LK",u8"tcy_IN",u8"te_IN",u8"tg_TJ",u8"the_NP",u8"th_TH",u8"tig_ER",u8"ti_ER",u8"ti_ET",u8"tk_TM",u8"tl_PH",u8"tn_ZA",u8"to_TO",u8"tpi_PG",u8"tr_CY",u8"tr_TR",u8"ts_ZA",u8"tt_RU",u8"tt_RU_iqtelif",u8"ug_CN",u8"uk_UA",u8"unm_US",u8"ur_IN",u8"ur_PK",u8"uz_UZ",u8"uz_UZ_cyrillic",u8"ve_ZA",u8"vi_VN",u8"wae_CH",u8"wal_ET",u8"wa_BE",u8"wa_BE_euro",u8"wo_SN",u8"xh_ZA",u8"yi_US",u8"yo_NG",u8"yue_HK",u8"yuw_PG",u8"zh_CN",u8"zh_HK",u8"zh_SG",u8"zh_TW",u8"zu_ZA",};
	using namespace fast_io::mnp;
	for(auto fnm : files)
	{
		fast_io::l10n loc(reinterpret_cast<char const*>(fnm));
		fast_io::iso8601_timestamp tsp{utc(posix_clock_gettime(fast_io::posix_clock_id::realtime))};
		print(imbue(loc,obf),chvw(reinterpret_cast<char const*>(fnm)),
		"\niso 8601:\t",tsp,"\n"
		"d_t_fmt:\t",d_t_fmt(tsp),
		"d_fmt:\t",d_fmt(tsp),
		"t_fmt:\t",t_fmt(tsp),
		"t_fmt_ampm:\t",t_fmt_ampm(tsp),
		"t_fmt:\t",t_fmt(tsp),
		"date_fmt:\t",date_fmt(tsp),
		"era_d_t_fmt:\t",era_d_t_fmt(tsp),
		"era_d_fmt:\t",era_d_fmt(tsp),
		"era_t_fmt:\t",era_t_fmt(tsp),
		"\n\n\n-----------------------------------------------------------------------------\n");
	}
}
// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The KSOC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "util.h"
#include "utilstrencodings.h"

#include <boost/assign/list_of.hpp>

#include <assert.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=1776d4804936840d61dda3ebb5d069bf9f9614dc88ee7da1826e330afec120e5, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000,
 * hashMerkleRoot=3a7112c2e43c7e59f5ada92631770b6c8e2bd1dfcde8670eaf9169dc321049f7, nTime=1539704377, nBits=1e0ffff0, nNonce=57612, vtx=1)
 * CTransaction(hash=3a7112c2e4, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *   CTxIn(COutPoint(0000000000000000000000000000000000000000000000000000000000000000, 4294967295),
 * coinbase 04ffff001d01044c514e592054696d65732031302f4f63742f3230313820487572726963616e65204d69636861656c204c69766520557064617465733a2053746f726d20557067726164656420746f2043617465676f72792033)
 *  CTxOut(nValue=0.00000000, scriptPubKey=04ab8dbce32c7daa6ff8a7d43794de)
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "NY Times 10/Oct/2018 Hurricane Michael Live Updates: Storm Upgraded to Category 3";
    const CScript genesisOutputScript = CScript() << ParseHex("04ab8dbce32c7daa6ff8a7d43794de3402a4e4f8909140d9d687624db0899ccc532c86b2310a9c10ac35fc3c7fdd5e7d5997928c7dfdebc568039790f6c141c0e8") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("00000cd060c1a502ca5d2cc40a399d18d05b8f9836cd9831e4237e87a6a8ed6a"))
    (500, uint256S("7c53d16e6307923395ecccc4c9a80b508b9741368508c4cd2cca1ecc15a4f3c8"))
    (1000, uint256S("fe2d2c754aa6f913fa0a423edb85ba158cb37c1dd3c69a8be203e9a7ae54f1ac"))
    (5000, uint256S("64a1a14799fd2b273b692f5b658105f5796e3e1077eebca388c1dd68b63e5dfb"))
    (10000, uint256S("b689067d2efb791cedb85b9590572e1138ff05dcafb1677ef5e0a1f4759f767a"))
    (15000, uint256S("03ca25d201d71b9a2bfa43111331c48a390752b4c86be4e6372763ad1ddea88a"))
    (20000, uint256S("451180ed1fb008cda20fb1f894a89af90e6f2ed26e6dc7b1290bb896533e2dcd"))
    (28000, uint256S("167b3862ada8611d7858fe435e9eff988f18bbf1914dba0d2e6d08f3c67e5a41"))
    (35000, uint256S("f9776ee853a97b5dbe601794b63648b6213b037ba19c428df574ba91f1084fd5"))
    (42000, uint256S("d08173f92812bdedb746a4cda100fd6328e3fb2fd36e0b61c7ebe1c5952bb2fe"))
    (45075, uint256S("98c75c40a0437f3d60b92a30cd5dd4da81edaa5a7c8bb99c94002a1f196fb22f"))
    (45082, uint256S("77957b9a7f5194f235a2c93653900aed254775e1ae0d7d6c0bc78c04aef413be")) //height of 1st split && procotol enforcemnt 
    (45900, uint256S("396e1ba5422701175132bda80e956d518c9f8b06468493f764fdd9006432815e"))
    (46001, uint256S("b7ffd9a47aabfae0253bfc7830816c28276dd192798921766882f316a76ace70")) //height of 2st split
    (46204, uint256S("b04230c4675d0fb266e97dcb6bd3b4007366277f6a0408cc76e201d4eb24cd9c"))
    (46720, uint256S("c099044687f783e8ea6df6df9cb642e8bc6de9ffeb5814cbac0905b9672277ae"))
    (50000, uint256S("3b36a0eb2b37e93dcbff158a9ebda75d018c02e0aa5906efaf8f40e6b6eb1b5f"))
    (55000, uint256S("ebe61ad4f450202331ac0afc5342a7c8aa5fe0ad32c1e752b15d447e2a3d371a"))
    (65000, uint256S("bd447d3722548b2a9092dd68a4735114c7bc352f020d74c903ddb8b488cfc4c6"))
    (70000, uint256S("0954ab42572595e5b04853d6d5e0f029c22d439f643c66427d4ad6dc0e27cacb"))
    (85000, uint256S("53353363d9ee7f1f1f3d35feb1f81175044fe70a10e9f04bd658ce066a5e4f6e"))
	 (91000, uint256S("afff8365e4a3c3f39c28c0b4b7bff09beef158982b76dccd7307de4c885ab2e1"))    
    ;

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1625016810, // * UNIX timestamp of last checkpoint block
    43812,      // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256S("1776d4804936840d61dda3ebb5d069bf9f9614dc88ee7da1826e330afec120e5"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1539704377,
    100,
    2000};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256S("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        genesis = CreateGenesisBlock(1539704377, 1591358, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("00000cd060c1a502ca5d2cc40a399d18d05b8f9836cd9831e4237e87a6a8ed6a"));
        assert(genesis.hashMerkleRoot == uint256("3a7112c2e43c7e59f5ada92631770b6c8e2bd1dfcde8670eaf9169dc321049f7"));
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // KSOC starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 20;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 10;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 2;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 1000000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60;
        consensus.nStakeMinDepth = 6;
        consensus.nTargetTimespan = 2 * 60;
        consensus.nTargetTimespanV2 = 2 * 60;
        consensus.nTargetSpacing = 2 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.strObfuscationPoolDummyAddress = "KNTPdJ1yFDpqmY4dM5Gs8sg9kp5QMAwXBu";
	consensus.strDevFeeAddress = "KQYptvmcUGoxcdtZpwtB8RrSCe7x2sqtgs";
	consensus.strDevFeeAddress1 = "KF3aHMg1FDZ8Embg4vNzJ9uMYd99fh3vhc";

        // spork keys
        consensus.strSporkPubKey = "038e2aaa12d0bd6824c90a7fa4e7b6c379b5c9bd6517bea009bddecccac9014b6c";

        // height-based activations
        consensus.height_last_PoW = 150;				 // 0000000c0a0f8be6edf60d7dbda06ecdbeecfa7d1241e5458aa68146b2551e8a
        consensus.height_RHF = 250; 					 // 8ab18a2bc9d3b0e872b36a22b14ce872c5090e5930728c2e46b33079b71fe8f3
        consensus.height_last_ZC_AccumCheckpoint = 150;			 // 0000000c0a0f8be6edf60d7dbda06ecdbeecfa7d1241e5458aa68146b2551e8a
        consensus.height_start_BIP65 = consensus.height_RHF;             // 8ab18a2bc9d3b0e872b36a22b14ce872c5090e5930728c2e46b33079b71fe8f3
        consensus.height_start_MessSignaturesV2 = consensus.height_RHF;  // TimeProtocolV2, Blocks V7 and newMessageSignatures
        consensus.height_start_StakeModifierNewSelection = 1;
        consensus.height_start_StakeModifierV2 = std::numeric_limits<int>::max();
        consensus.height_start_TimeProtoV2 = std::numeric_limits<int>::max();
        consensus.height_start_ZC = 0;
        consensus.height_start_ZC_PublicSpends = consensus.height_RHF;
        consensus.height_start_ZC_SerialRangeCheck = consensus.height_RHF;
        consensus.height_start_ZC_SerialsV2 = consensus.height_RHF;

        // Zerocoin-related params
        consensus.ZC_Modulus = "d59f1d99dae2770f40fb82066b6f69bb0b3783113505ecf4d958a6021d7204a8612d7c824741ac69cbf426ba4056a0598f2683c54a72c9162821864da23add323b9af365c63d1c60af802a15c3961c4a23a0a4b8f8d0cd681faf9ff5f308a9d8348993a7f5e2560bdc4274aaa670878562ad8774c7fa15ec449385a7e3f2621b152e1f9978890cf02058d3f00d7ed1fc2fba76fe2b8358205dec3f0bd0b648b995f84b74e34ae77a2c134033075cf966b4339f028e039ce8200e279bd0169cf5994a4b135699280fa7be8f0328cfcaa1f7dc7cabe18ba0ec6f42e00792b3f128ec64fee8eb9306b871f6514946649d3fa2247c62ecd5050914570bb35b035fa80ef0995006790eb5ef2e383e7919b7e1aea89f59917c1a7adfdb1a73239c09e191cdde217c53ba0bf96ac9c265054aef811da8b51b1b3ea31d96f5d1ab9acf87363be80f42acf7353b3c4a5297eb3f5676f04b987a3144c5b04d1f6f3fdec243bab3fa2f463a1c50be50b49c156c421befad74c9b6f4367149163d3796355331";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
	consensus.ZC_TimeStart = 1547096400;        // October 17, 2017 4:30:00 AM
        consensus.ZC_WrappedSerialsSupply = 4131563 * COIN;   // zerocoin supply at height_last_ZC_WrappedSerials

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 151;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 151;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 863787;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 1153160;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 1808634;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 1880000;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = consensus.height_RHF;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 2153200;
        consensus.vUpgrades[Consensus::UPGRADE_V5_DUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock          = uint256S("0x001");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock       = uint256S("0x001");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock       = uint256S("0x001"); //tochange
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].hashActivationBlock   = uint256S("0x001");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock        = uint256S("0x001");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock        = uint256S("0x001");
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbd;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xd8;
        nDefaultPort = 20555;

        // Note that of those with the service bits flag, most only support a subset of possible options
        //vSeeds.push_back(CDNSSeedData("ksoc-network.com", "seeds.ksoc-network.com"));
        //vSeeds.push_back(CDNSSeedData("ksoccoin.com", "seeds.ksoc.ksoccoin.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 45);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 107);
	base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 107);     // starting with 'k'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x03)(0xbe).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ps";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviews";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "pivks";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]         = "p-secret-spending-key-main";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";

        genesis = CreateGenesisBlock(1454124731, 2402015, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));
        //assert(genesis.hashMerkleRoot == uint256S("0x1b2ef6e2f28be914103a277377ae7729dcd125dfeb8bf97bd5964ba72b6dc39b"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // KSOC starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on testnet)
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        // spork keys
        consensus.strSporkPubKey = "04E88BB455E2A04E65FCC41D88CD367E9CCE1F5A409BE94D8C2B4B35D223DED9C8E2F4E061349BA3A38839282508066B6DC4DB72DD432AC4067991E6BF20176127";
        consensus.strSporkPubKeyOld = "04A8B319388C0F8588D238B9941DC26B26D3F9465266B368A051C5C100F79306A557780101FE2192FE170D7E6DEFDCBEE4C8D533396389C0DAFFDBC842B002243C";
        consensus.nTime_EnforceNewSporkKey = 1566860400;    //!> August 26, 2019 11:00:00 PM GMT
        consensus.nTime_RejectOldSporkKey = 1569538800;     //!> September 26, 2019 11:00:00 PM GMT

        // height based activations
        consensus.height_last_ZC_AccumCheckpoint = 1106090;
        consensus.height_last_ZC_WrappedSerials = -1;
        consensus.height_start_InvalidUTXOsCheck = 999999999;
        consensus.height_start_ZC_InvalidSerials = 999999999;
        consensus.height_start_ZC_SerialRangeCheck = 1;
        consensus.height_ZC_RecalcAccumulators = 999999999;

        // validation by-pass
        consensus.nKsocBadBlockTime = 1489001494; // Skip nBit validation of Block 201 per PR #915
        consensus.nKsocBadBlockBits = 0x1e0a20bd; // Skip nBit validation of Block 201 per PR #915

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 1501776000;
        consensus.ZC_WrappedSerialsSupply = 0;   // WrappedSerials only on main net

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 201;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 51197;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 201576;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 444020;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 851019;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 1106100;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 1214000;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 1347000;
        consensus.vUpgrades[Consensus::UPGRADE_V5_DUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock =
                uint256S("0x258c489f42f03cb97db2255e47938da4083eee4e242853c2d48bae2b1d0110a6");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock =
                uint256S("0xfcc6a4c1da22e4db2ada87d257d6eef5e6922347ca1bb7879edfee27d24f64b5");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock =
                uint256S("0xc54b3e7e8b710e4075da1806adf2d508ae722627d5bcc43f594cf64d5eef8b30");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock =
                uint256S("0x1822577176173752aea33d1f60607cefe9e0b1c54ebaa77eb40201a385506199");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock =
                uint256S("0x30c173ffc09a13f288bf6e828216107037ce5b79536b1cebd750a014f4939882");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0x45;
        pchMessageStart[1] = 0x76;
        pchMessageStart[2] = 0x65;
        pchMessageStart[3] = 0xba;
        nDefaultPort = 51474;

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "ksoc-testnet.seed.fuzzbawls.pw", true));
        vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "ksoc-testnet.seed2.fuzzbawls.pw", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet ksoc addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet ksoc script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 73);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet ksoc BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet ksoc BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet ksoc BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ptestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviewtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "pivktestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]         = "p-secret-spending-key-test";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";

        genesis = CreateGenesisBlock(1454124731, 2402015, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));
        //assert(genesis.hashMerkleRoot == uint256S("0x1b2ef6e2f28be914103a277377ae7729dcd125dfeb8bf97bd5964ba72b6dc39b"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // KSOC starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on regtest)
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 2;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "043969b1b0e6f327de37f297a015d37e2235eaaeeb3933deecd8162c075cee0207b13537618bde640879606001a8136091c62ec272dd0133424a178704e6e75bb7";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height based activations
        consensus.height_last_ZC_AccumCheckpoint = 310;     // no checkpoints on regtest
        consensus.height_last_ZC_WrappedSerials = -1;
        consensus.height_start_InvalidUTXOsCheck = 999999999;
        consensus.height_start_ZC_InvalidSerials = 999999999;
        consensus.height_start_ZC_SerialRangeCheck = 300;
        consensus.height_ZC_RecalcAccumulators = 999999999;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 10;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 10;
        consensus.ZC_TimeStart = 0;                 // not implemented on regtest
        consensus.ZC_WrappedSerialsSupply = 0;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 251;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 251;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 300;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 300;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 400;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 251;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_DUMMY].nActivationHeight       = 300;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nDefaultPort = 51476;

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}

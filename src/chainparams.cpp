// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2022 The Lunarium Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
(0,  uint256("0x0000be6cab2a2ca03fae8266bb2bbd59bc21b4b94ec5107bb871bb4fbfaef889"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1656485290, // * UNIX timestamp of last checkpoint block
    104,     // * total number of transactions between genesis and last checkpoint
           //   (the tx=... number in the SetBestChain debug.log lines)
    1400        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x0000be6cab2a2ca03fae8266bb2bbd59bc21b4b94ec5107bb871bb4fbfaef889"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1656485290,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x0000be6cab2a2ca03fae8266bb2bbd59bc21b4b94ec5107bb871bb4fbfaef889"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1656485290,
    0,
    250};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment. 
         */
        pchMessageStart[0] = 0xC1;
        pchMessageStart[1] = 0x6c;
        pchMessageStart[2] = 0x5f;
        pchMessageStart[3] = 0x64;
        vAlertPubKey = ParseHex("04c826c3c83aed95452e301313d0b42409f0a79d464a0f26ecf8cf9258e33ee93b9c5b547a84739f0d06cad405aa94044bdb55922969709935b3e4582f10b8a6ab");
        nDefaultPort = 42524; 
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Lunarium starting difficulty is 1 / 2^12
        nMaxReorganizationDepth = 100;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 60;  // XLN: 1 minutes
        nMaturity = 59;
        nMaxMoneyOut = 1000000000 * COIN;

        nSubsidyHalvingBlock = 1000;
        strDevFeeAddress = "Le8UrPvpAtWCYPiTQw8emHTtba8LPyFJgR";

        /** Height or Time Based Activations **/
        nLastPOWBlock = 200;
        nModifierUpdateBlock = 0;
        nZerocoinStartHeight = 0;
        nZerocoinStartTime = 1547096400; // Genesis time
        nBlockZerocoinV2 = 20;

        const char* pszTimestamp = "New Lunarium Genesis Block mined by Team in 06/2022";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04063eafa23cf4bc659e7dadbeb1fe3923a8c1923b5b7f40a0ceb7ef78001b856e76fc058d9150838dade408e794e7e43f4045e45385026df815aecbabab231013") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 4;
        genesis.nTime = 1656485290;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 15879;


        hashGenesisBlock = genesis.GetHash();
        //printf("Genesis mainnet - %s\n",hashGenesisBlock.ToString().c_str());
        //printf("Merkle mainnet - %s\n",genesis.hashMerkleRoot.ToString().c_str());


        assert(hashGenesisBlock == uint256("0x0000be6cab2a2ca03fae8266bb2bbd59bc21b4b94ec5107bb871bb4fbfaef889"));
        assert(genesis.hashMerkleRoot == uint256("0x35fd0b5bab0a2d37eec3e67b11e8eb8ae1b75fbf5a9bbf997b226645cbc2e452"));

        vSeeds.push_back(CDNSSeedData("seed1.lunariumcoin.com", "seed1.lunariumcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed2.lunariumcoin.com", "seed2.lunariumcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed3.lunariumcoin.com", "seed3.lunariumcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed4.lunariumcoin.com", "seed4.lunariumcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed5.lunariumcoin.com", "seed5.lunariumcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed6.lunariumcoin.com", "seed6.lunariumcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed7.lunariumcoin.com", "seed7.lunariumcoin.com"));
        vSeeds.push_back(CDNSSeedData("seed8.lunariumcoin.com", "seed8.lunariumcoin.com"));
        vSeeds.push_back(CDNSSeedData("45.94.209.55", "45.94.209.55"));
        vSeeds.push_back(CDNSSeedData("84.54.23.196", "84.54.23.196"));
        vSeeds.push_back(CDNSSeedData("164.68.123.250", "164.68.123.250"));
        vSeeds.push_back(CDNSSeedData("167.86.80.59", "167.86.80.59"));
         

        // Lunarium addresses start with 'L'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 48);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 49);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 25);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0xB2)(0x47)(0x46).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0xB2)(0x43)(0x08).convert_to_container<std::vector<unsigned char> >();
                //BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x02)(0x62).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "0471f9913dc8f1b8c333cb9f5170f8375fc5306f7e008df10b21a7fb4b8a481ef8dcba1958789278d6d8e27905bed0774d5a344a11bbff129b90c56c187d2c6ea7";
        strObfuscationPoolDummyAddress = "LPj1DfwPQuJbJUbJXMQZwPUsAAL7fsjX98"; 
        nStartMasternodePayments = 1547119800; // historical param

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zxln to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
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
        pchMessageStart[0] = 0xC3;
        pchMessageStart[1] = 0x6f;
        pchMessageStart[2] = 0x81;
        pchMessageStart[3] = 0x5a;
        vAlertPubKey = ParseHex("04063eafa23cf4bc659e7dadbeb1fe3923a8c1923b5b7f40a0ceb7ef78001b856e76fc058d9150838dade408e794e7e43f4045e45385026df815aecbabab231013");
        nDefaultPort = 27293;
        nMinerThreads = 0;
        nTargetSpacing = 2 * 60;
        nLastPOWBlock = 500;
        nMaturity = 15;
        nModifierUpdateBlock = 0;
        nMaxMoneyOut = 1000000000 * COIN;
        nZerocoinStartHeight = 15;
        nZerocoinStartTime = 1547096400;
        nBlockZerocoinV2 = 15;

        nSubsidyHalvingBlock = 1000;
        

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1656485290;
        genesis.nNonce = 15879;

        hashGenesisBlock = genesis.GetHash();

        

        assert(hashGenesisBlock == uint256("0x0000be6cab2a2ca03fae8266bb2bbd59bc21b4b94ec5107bb871bb4fbfaef889"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 85); // Testnet lunarium addresses start with 'b'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 86);  // Testnet lunarium script addresses start with 'b' or 'c'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 86);     // Testnet private keys start with 'b' or 'c'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x02)(0x6e).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "049695108933ccaf2ec029fa815bccb977993927584f726ba13d0cf28896099772358901567c746c3bed54f53d75e8ef94eb6050088dc3951c635af5940c650a20";
        strObfuscationPoolDummyAddress = "LPj1DfwPQuJbJUbJXMQZwPUsAAL7fsjX98"; 
        nStartMasternodePayments = 1547119740 + 500 * 120;
        nBudget_Fee_Confirmations = 3;

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
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x31;
        pchMessageStart[1] = 0x45;
        pchMessageStart[2] = 0x27;
        pchMessageStart[3] = 0xA9;
        nMinerThreads = 1;
        nTargetSpacing = 1 * 60;
        nDefaultPort = 37293;

        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1656485290;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 15879;

        hashGenesisBlock = genesis.GetHash();


        //printf("Genesis RegTest - %s\n",hashGenesisBlock.ToString().c_str());
        //printf("Merkle RegTest - %s\n",genesis.hashMerkleRoot.ToString().c_str());

        assert(hashGenesisBlock == uint256("0x0000be6cab2a2ca03fae8266bb2bbd59bc21b4b94ec5107bb871bb4fbfaef889"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 49293;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingBlock(int anSubsidyHalvingBlock) { nSubsidyHalvingBlock = anSubsidyHalvingBlock; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

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
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
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

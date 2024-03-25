/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/25 03:19:54 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Request.hpp"

std::vector<Server> Request::servers;

/* *************************** Constructors *************************** */

Request::Request()
{
	this->status = OK;
	this->detectHost = 0;
	this->bodyDone = false;
	this->foundUri = false;
	this->receivecount = 0;
	this->sizeBoundary = 0;
	this->contentLength = 0;
	this->multipart = false;
	this->_setLength = false;
	this->isComplete = false;
	this->remainingChunk = 0;
	this->headersDone = false;
	this->requestLineDone = false;
	this->remainingChunkLength = 0;
	this->_chunkedComplete = false;
	this->requestIscomplete = false;
	this->requestInProgress = false;
	this->_requestIsWellFormed = false;
	this->requestLineInProgress = false;
	this->_path.clear();
	this->_body.clear();
	this->_params.clear();
	this->headers.clear();
	this->requestData.clear();
	this->boundaryName.clear();
	this->_chunkedName.clear();
	this->requestVector.clear();
	this->requestLineData.clear();
}

Request::Request(const Request& obj)
{
	*this = obj;
}

Request& Request::operator=(const Request& obj)
{
	if (this != &obj)
	{
		this->_body = obj._body;
		this->_path = obj._path;
		serv_end = obj.serv_end;
		this->status = obj.status;
		serv_begin = obj.serv_begin;
		this->_params = obj._params;
		this->headers = obj.headers;
		this->serverCTX = obj.serverCTX;
		this->locationCTX = obj.locationCTX;
		this->bodyDone = obj.bodyDone;
		this->foundUri = obj.foundUri;
		this->multipart = obj.multipart;
		this->_setLength = obj._setLength;
		this->isComplete = obj.isComplete;
		this->detectHost = obj.detectHost;
		this->requestLine = obj.requestLine;
		this->headersDone = obj.headersDone;
		this->requestData = obj.requestData;
		this->receivecount = obj.receivecount;
		this->_chunkedName = obj._chunkedName;
		this->sizeBoundary = obj.sizeBoundary;
		this->boundaryName = obj.boundaryName;
		this->contentLength = obj.contentLength;
		this->requestVector = obj.requestVector;
		this->remainingChunk = obj.remainingChunk;
		this->requestLineDone = obj.requestLineDone;
		this->_chunkedComplete = obj._chunkedComplete;
		this->requestIscomplete = obj.requestIscomplete;
		this->requestInProgress = obj.requestInProgress;
		this->_requestIsWellFormed = obj._requestIsWellFormed;
		this->remainingChunkLength = obj.remainingChunkLength;
		this->requestLineInProgress = obj.requestLineInProgress;
	}
	return (*this);
}

Request::~Request()
{
}

/* *************************** getters *************************** */

const String&	Request::getBody() const
{
	return (_body);
}

const std::map<String, String>&	Request::getHeaders() const
{
	return (_headers);
}

const std::map<String, String>&	Request::getRequestLine() const
{
	return (requestLine);
}

const int&	Request::getStatus() const
{
	return (status);
}

void	Request::setStatus(int status)
{
	this->status = status;
}

const bool& Request::getRequestIsWellFormed() const
{
	return (_requestIsWellFormed);
}

const bool& Request::getBodyDone() const
{
	return (bodyDone);
}

const bool& Request::getHeadersDone() const
{
	return (headersDone);
}

const String Request::getHeaderByName(const String& name) const
{
	Utils::toLower((String&)name);
	try
	{
		return (_headers.at(name));
	}
	catch(const std::exception& e)
	{
		if (name == "connection")
			return ("keep-alive");
	}
	return ("");
}

const ServerContext& Request::getServerCTX(void) const
{
	return serverCTX;
}

const LocationContext& Request::getLocationCtx(void) const
{
	return locationCTX;
}

const bool& Request::getRequestLineDone() const
{
	return (requestLineDone);
}

const bool& Request::getFoundUri() const
{
	return (foundUri);
}

bool	Request::isDone(void) const
{
	return (requestIscomplete);
}

bool	Request::hostIsDetected(void) const
{
	return (detectHost);
}

const String Request::getMethod() const
{
	try{
		return requestLine.at("method");
	}
	catch(const std::exception& e) {}
	return ("");
}

const String Request::getHost() const
{
	try{
		return (_headers.at("host"));
	}
	catch(const std::exception& e){}
	return ("");
}

const std::string Request::getRequestPath() const
{
	try{
		return (requestLine.at("path"));
	}
	catch(const std::exception& e){}
	return ("");
}

/* *************************** methods *************************** */

void	Request::resetRequest()
{
	this->status = OK;
	this->detectHost = 0;
	this->bodyDone = false;
	this->foundUri = false;
	this->receivecount = 0;
	this->sizeBoundary = 0;
	this->contentLength = 0;
	this->multipart = false;
	this->remainingChunk = 0;
	this->_setLength = false;
	this->isComplete = false;
	this->headersDone = false;
	this->requestLineDone = false;
	this->remainingChunkLength = 0;
	this->_chunkedComplete = false;
	this->requestIscomplete = false;
	this->requestInProgress = false;
	this->_requestIsWellFormed = false;
	this->requestLineInProgress = false;
	this->_path.clear();
	this->_body.clear();
	this->_params.clear();
	this->headers.clear();
	this->requestData.clear();
	this->boundaryName.clear();
	this->_chunkedName.clear();
	this->requestLine.clear();
	this->headers.clear();
	this->requestData.clear();
	this->requestVector.clear();
}

void	Request::selectServerContext(const String& host)
{
	servers_it it, end;
	StringVector::iterator	name;

	end = Request::servers.begin() + serv_end;
	it  = Request::servers.begin() + serv_begin;
	while (it != end)
	{
		StringVector hosts(it->getServerNames());
		name = std::find(hosts.begin(), hosts.end(), host);
		if (name != hosts.end())
		{
			serverCTX = it->getServerCTX();
			break;
		}
		it++;
	}
}

void	Request::setServerCTXEnd(size_t i)
{
	serv_end = i;
}

void	Request::setServerCTXBegin(size_t i)
{
	serv_begin = i;
}

//main function to parse the request
void	Request::parseRequest(const std::string& receivedRequest, ServerContext serverCTX)
{
	if (receivedRequest.empty())
		return;
	this->serverCTX = serverCTX;
	std::srand(time(NULL));
	if (!this->requestLineDone || !this->headersDone || !this->_requestIsWellFormed)
	{
		/* L body i9dar ikoun f GET wla delete 3la 7ssab CGI !!! */
		if (takingRequests(receivedRequest))
		{
			if (detectHost > 1)
			{
				this->status = BadRequest;
				requestIscomplete = true;
			}
			return;
		}
	}
	if (this->requestLine["method"] == "POST" && !this->bodyDone
		&& this->_requestIsWellFormed && this->status == 200 && foundUri)
	{
		setUploadingPath();
		if (status != 200)
			return;
		if (this->receivecount > 1)
		{
			if (receivedRequest.front() == CR && this->requestInProgress)
				return;
			this->_body = receivedRequest;
		}
		if (!this->_body.empty())
			parseBody();
	}
}


/*

8d2^M$
¾¡©y^H#Y�M-^Yç�M-^T^UU�M-^C�M-^Dc³¬å·s�M-^T�M-^_�M-^N²,�M-^XTcm¹m^_�M-^Hªý�M-^PH�M-^_«¿^Y)@�M-^AM�M-^D¹�M-^_[|�M-^Z�M-^C^Sz�M-^I^T^M$
�M-^Fi·2îW�M-^KTo�M-^O�M-^GEN�M-^[å�M-^Fs²®�M-^P^¦ù«è«�M-^K^�M-^[aèc5�M-^]�M-^R^â�M-^LRv"±�M-^T�M-^T[Y(T�M-^U^Z<# 7�M-^Q^\v^X^T?g�M-^^�M-^\M^H*sw7·ã^Gi^Qyl^Zc/^UXe&úê¨:�M-^PP¢?'UB,^B?�M-^U^Q�M-^Js�M-^T["�M-^AeOní¼l^A^K^Q^RXñò[^?�M-^@Bj�M-^Yç>·ëg|E�M-^C�M-^Da�M-^Eü^^�M-^R²�M-^LAë@�M-^_õ¿�M-^Sg�M-^Pî^Cà:åøº°     ÿU�M-^S÷ú,­"^âbUü¼Ióá�M-^My¢ã�M-^MR*é-º�M-^V^Gþ�M-^Xä�M-^F�M-^G�M-^P[^OWE8#�M-^G,rC*ê^YG�M-^Fµê^M$
c^SZw�M-^R�M-^[<�M-^L^Rý<�M-^]^NpLE�M-^I�M-^PP^W�M-^N9y!^D�M-^X^U^H�M-^N^]§\ìm°e©�M-^U^\Q�M-^EãD4�M-^\ª'-�M-^Fª�M-^W^[^C&µ¾e�M-^D^N5©^?¤^~Xü»�M-^E@¼�M-^[^HAO*4s¼�M-^T�M-^G2�M-^\J�M-^S�M-^I�M-^]�M-^U+�M-^E�M-^Bv�M-^W`^F�M-^X�M-^OY9�M-^Y�M-^X%^R^P�M-^]/íj^V�M-^YçS�M-^Qk¬tw>�M-^YøJ�M-^Aõ^E�M-^ZpB^Q^Ho^^Y�M-^^j�M-^A^Tö�M-^Z�M-^Nm�M-^X¥^[ãvë^X^ACù�M-^H�M-^Dü�M-^Mìsó�M-^^²|m�M-^H¼�M-^Cknò1ìA�M-^Yñ�M-^Y^[^Fì^L�M-^G^Uíg�M-^H^]�M-^U>s^C^Léï³�M-^\7^S�M-^[5^B�M-^[l^E�M-^Q^Z�M-^T�M-^OH�M-^S^W�M-^R^H"u^A�M-^B�M-^Q "ê^]è�M-^K�M-^C �M-^H�M-^B<*^N«ª¹"ëD�M-^Y÷e\^T�M-^R^[9^F÷ezfê8d2^M$
¾¡©y^H#Y�M-^Yç�M-^T^UU�M-^C�M-^Dc³¬å·s�M-^T�M-^_�M-^N²,�M-^XTcm¹m^_�M-^Hªý�M-^PH�M-^_«¿^Y)@�M-^AM�M-^D¹�M-^_[|�M-^Z�M-^C^Sz�M-^I^T^M$
�M-^Fi·2îW�M-^KTo�M-^O�M-^GEN�M-^[å�M-^Fs²®�M-^P^¦ù«è«�M-^K^�M-^[aèc5�M-^]�M-^R^â�M-^LRv"±�M-^T�M-^T[Y(T�M-^U^Z<# 7�M-^Q^\v^X^T?g�M-^^�M-^\M^H*sw7·ã^Gi^Qyl^Zc/^UXe&úê¨:�M-^PP¢?'UB,^B?�M-^U^Q�M-^Js�M-^T["�M-^AeOní¼l^A^K^Q^RXñò[^?�M-^@Bj�M-^Yç>·ëg|E�M-^C�M-^Da�M-^Eü^^�M-^R²�M-^LAë@�M-^_õ¿�M-^Sg�M-^Pî^Cà:åøº°     ÿU�M-^S÷ú,­"^âbUü¼Ióá�M-^My¢ã�M-^MR*é-º�M-^V^Gþ�M-^Xä�M-^F�M-^G�M-^P[^OWE8#�M-^G,rC*ê^YG�M-^Fµê^M$
c^SZw�M-^R�M-^[<�M-^L^Rý<�M-^]^NpLE�M-^I�M-^PP^W�M-^N9y!^D�M-^X^U^H�M-^N^]§\ìm°e©�M-^U^\Q�M-^EãD4�M-^\ª'-�M-^Fª�M-^W^[^C&µ¾e�M-^D^N5©^?¤^~Xü»�M-^E@¼�M-^[^HAO*4s¼�M-^T�M-^G2�M-^\J�M-^S�M-^I�M-^]�M-^U+�M-^E�M-^Bv�M-^W`^F�M-^X�M-^OY9�M-^Y�M-^X%^R^P�M-^]/íj^V�M-^YçS�M-^Qk¬tw>�M-^YøJ�M-^Aõ^E�M-^ZpB^Q^Ho^^Y�M-^^j�M-^A^Tö�M-^Z�M-^Nm�M-^X¥^[ãvë^X^ACù�M-^H�M-^Dü�M-^Mìsó�M-^^²|m�M-^H¼�M-^Cknò1ìA�M-^Yñ�M-^Y^[^Fì^L�M-^G^Uíg�M-^H^]�M-^U>s^C^Léï³�M-^\7^S�M-^[5^B�M-^[l^E�M-^Q^Z�M-^T�M-^OH�M-^S^W�M-^R^H"u^A�M-^B�M-^Q "ê^]è�M-^K�M-^C �M-^H�M-^B<*^N«ª¹"ëD�M-^Y÷e\^T�M-^R^[9^F÷ezfê�M-^Fi·2îW�M-^KTo�M-^O�M-^GEN�M-^[å�M-^Fs²®�M-^P^¦ù«è«�M-^K^�M-^[aèc5�M-^]�M-^R^â�M-^LRv"±�M-^T�M-^T[Y(T�M-^U^Z<# 7�M-^Q^\v^X^T?g�M-^^�M-^\M^H*sw7·ã^Gi^Qyl^Zc/^UXe&úê¨:�M-^PP¢?'UB,^B?�M-^U^Q�M-^Js�M-^T["�M-^AeOní¼l^A^K^Q^RXñò[^?�M-^@Bj�M-^Yç>·ëg|E�M-^C�M-^Da�M-^Eü^^�M-^R²�M-^LAë@�M-^_õ¿�M-^Sg�M-^Pî^Cà:åøº°     ÿU�M-^S÷ú,­"^âbUü¼Ióá�M-^My¢ã�M-^MR*é-º�M-^V^Gþ�M-^Xä�M-^F�M-^G�M-^P[^OWE8#�M-^G,rC*ê^YG�M-^Fµê^M$
c^SZw�M-^R�M-^[<�M-^L^Rý<�M-^]^NpLE�M-^I�M-^PP^W�M-^N9y!^D�M-^X^U^H�M-^N^]§\ìm°e©�M-^U^\Q�M-^EãD4�M-^\ª'-�M-^Fª�M-^W^[^C&µ¾e�M-^D^N5©^?¤^~Xü»�M-^E@¼�M-^[^HAO*4s¼�M-^T�M-^G2�M-^\J�M-^S�M-^I�M-^]�M-^U+�M-^E�M-^Bv�M-^W`^F�M-^X�M-^OY9�M-^Y�M-^X%^R^P�M-^]/íj^V�M-^YçS�M-^Qk¬tw>�M-^YøJ�M-^Aõ^E�M-^ZpB^Q^Ho^^Y�M-^^j�M-^A^Tö�M-^Z�M-^Nm�M-^X¥^[ãvë^X^ACù�M-^H�M-^Dü�M-^Mìsó�M-^^²|m�M-^H¼�M-^Cknò1ìA�M-^Yñ�M-^Y^[^Fì^L�M-^G^Uíg�M-^H^]�M-^U>s^C^Léï³�M-^\7^S�M-^[5^B�M-^[l^E�M-^Q^Z�M-^T�M-^OH�M-^S^W�M-^R^H"u^A�M-^B�M-^Q "ê^]è�M-^K�M-^C �M-^H�M-^B<*^N«ª¹"ëD�M-^Y÷e\^T�M-^R^[9^F÷ezfêS�M-^_%�M-^LT�M-^Oê"{;�M-^G¨ú^Dâ�M-^]WZ^H0^H¨^B@í-jé ,^NBçR^^Cù^?�M-^A^D/^^5:óï�M-^G>�M-^Hw�M-^Lvê�M-^Mý7�M-^G[u^GN^Y½�M-^T^_uQ^M$
0wI~G¤1çX©f^WeFE¥õ^X�M-^C�M-^D�M-^D�M-^@¾�M-^M·hGèk�M-^MùbH�M-^T?�M-^@^W¼ä¤�M-^[hd^^�M-^NSL�M-^H¢�M-^O¤#fV-ú·$~N�M-^\è(`�M-^Y¿�M-^C�M-^FjOUáh^S�M-^J�M-^Z�M-^O�M-^W^[�M-^N®�M-^R¹�M-^Kppb�M-^\/»§üV�M-^]¤V*c¿á�M-^F&{«à,¥�M-^R³f3�M-^F`»`N�M-^N6�M-^V2�M-^Jâ�M-^Fó¹^R^R�M-^R�M-^X�M-^^^U�M-^F{�M-^_�M-^Qn­^Bu6�M-^A�M-^[�M-^X¾¸^Z^K^U3^O�M-^BI^W�M-^JR�M-^Bïé¦¦2�M-^J�M-^Z�M-^So^H^^    l9b�M-^X�M-^P%J�M-^_^R¸¹a�M-^UD�M-^B j'£j}ù�M-^^S¦X�M-^IE�M-^\p)23^W còã�M-^G/ég¿�M-^Pø^A^Y/w+fL^D�M-^XKàqqT�M-^CaA^^]�M-^A�M-^^�M-^Füuê�M-^NJ�M-^^»'>@XhW�M-^M¯¹�M-^H:�M-^P�M-^An:î¹y-E�M-^L�M-^P�M-^^¾h^�M-^^'ùrúQ¥�M-^FjX,<�M-^_�M-^Xê£ì^HO^P®�M-^Xaw�M-^K71�M-^E^)`�M-^KNU�M-^C�M-^M,W�M-^Rr¸�M-^K�M-^W�M-^AO�M-^[W�M-^\=ºµ�M-^Lp¼^B&(þ�M-^F[}�M-^A²¯±x#g�M-^@EöSðo°ã�M-^V¡«~*.Y^R�M-^A00pN±õ�M-^D^M$
î�M-^F�M-^L�M-^]�M-^W³�M-^C^DgCêmº�M-^E�M-^Q¢�M-^V-hûu^V^K»â:�M-^G¢=�M-^I^K�M-^QN8¢ñ&�M-^Y^M$
­õ^Z^_{D^T�M-^Z�M-^N�M-^S¦^\^E0^U�M-^Nï�M-^IX�M-^X�M-^Qn< ã^R^]S�M-^H�M-^Vt�M-^X�M-^]¦D|^]�M-^Uº^\�M-^X{^B�M-^EYQ:§�M-^\�M-^[�M-^S�M-^Z�M-^G�M-^V�M-^]^X�M-^H«H�M-^J�M-^X^CVID�M-^T¤ô3A�M-^Rá&4ÿ¶Y²^Zj^R�M-^J�M-^\s÷tF*?:ä1î�M-^H�M-^A�M-^Wð^^»åû+�M-^E½�M-^B�M-^T^M$
�M-^F�M-^Dô^?¨�M-^_xe^\^D#l}e�M-^P�M-^Q�M-^W'^M$
�M-^Zi�M-^Q^R´�M-^G^P¹^A�M-^Z�M-^X_bKM,èZ�M-^@@�M-^L�M-^O�M-^E�M-^R�M-^C�M-^X�M-^Dmý�M-^_ë­5ú.�M-^L^?Q�M-^B¹�M-^L^W Y�M-^WE^Y�M-^P²y8Lôö�M-^@�M-^Ecoe,^]ð,î^A"k�M-^Ft^X^Tâç*^?^?Q�M-^^¿´^CB�M-^^�M-^[^D"O804²glGrE^?q*�M-^Ibð�M-^Eµ�M-^[v/g�M-^G8       l�M-^@�M-^J�M-^F�M-^X�M-^I-í^M$
f{[�M-^^®=aèè¬á(�M-^@:J¼�M-^Fõë7�M-^^óR.;�M-^CtV�M-^GL²�M-^A¯�M-^]�M-^Pö6­%ðð ¿÷ñ^Q�M-^E^\�M-^KA^V$�M-^\á�M-^O0mõ�M-^Mt^M$
à�M-^T�M-^Q�M-^N�M-^Bñ¢½^B�M-^U¦�M-^M^M$
�M-^@�M-^W_h^M$
^S�M-^]{�M-^Od³�M-^]�M-^RJ�M-^S�M-^Q^B^Eÿ¤07�M-^T^?·$�M-^U½°v�M-^E�M-^K1¾`W®�M-^W�M-^K2^Hª0�M-^U�M-^[�M-^G§æ¾�M-^DR�M-^Vo^RºS;³�M-^\_ª%�M-^ZF^Y�M-^Nã¡z�M-^\�M-^W�M-^_^BóP/&¶�M-^\^Q^_ç�M-^Múdr^]vñ�M-^T�M-^W�M-^L)J²KH^H^K;�M-^_�M-^B�M-^E^G¨E«�M-^_¬^^î^[^S'­6�M-^Z�M-^R      J�M-^@�M-^L�M-^Rs_^G`ln#�M-^I^U~¼�M-^I�M-^^�M-^Në�M-^O¼C%·fD�M-^Fý�M-^D �M-^Q¢^S2�M-^Z^[£�M-^Ní$½b�M-^D�M-^]|{^B~�M-^A~�M-^Igq�M-^U'á^M$
¤h�M-^P$c^F�M-^W?æZ¨õz^G�M-^Y¤6úí^M$
0^M$
^M$

*/
# ACNHRecomp Legacy 0.11.0 — experimento para Windows x64

Base: código-fonte ACNHRecomp enviado pelo usuário (commit 765aa5409c79a09d0e3f9ff977b54aabea763c9e, 17/11/2026). Projeto original MIT; veja LICENSE. Não é uma versão oficial nem uma compilação executável. Não inclui jogo, chaves ou arquivos do RAR de distribuição.

## Alterações
- Windows clang-cl: substituição de /arch:AVX2 por /clang:-march=x86-64-v2 e /clang:-ffp-contract=off. A configuração rejeita MSVC sem Clang porque o caminho de build desta edição usa clang-cl.
- Linux x86-64: mesmo baseline v2 (não é alvo do workflow experimental).
- Zig: flags x86_64_v2 para os módulos da biblioteca do jogo x64, objeto de detecção de CPU e link.
- Verificação de CPU do launcher: checagem dos recursos x86-64-v2 (SSE3, SSSE3, CMPXCHG16B, SSE4.1/4.2, POPCNT e LAHF/SAHF), não remoção indiscriminada do teste.
- ABI marcada com um identificador Legacy e revisão da biblioteca 5, para rejeitar e reconstruir DLLs antigas. Não misture os executáveis e DLLs oficiais com os novos. O Importer original ainda gera DLL v3.
- Workflow de build Windows baseado no arquivo do projeto original, com versão 0.11.0-legacy. Não foi executado no GitHub nesta sessão.

## Testes feitos nesta sessão
1. clang-cl 17 aceitou as novas flags; os macros SSE4.2 e POPCNT apareceram, e não apareceram AVX, AVX2 ou FMA.
2. A rotina de verificação de CPU isolada compilou e executou em Linux, em hardware do ambiente de análise (não no i7-2600).
3. O YAML do workflow foi analisado com sucesso.

Não foi feita compilação integral do Windows, execução do jogo, teste em Sandy Bridge ou auditoria de todas as bibliotecas externas. O SDK do Windows e a infraestrutura do build completo não estão disponíveis aqui. Instruções explícitas em dependências, bibliotecas de terceiros e ferramentas de shader podem ainda exigir CPUs mais novas. Não há garantia de desempenho ou compatibilidade.

## Como produzir os executáveis sem compilar no i7-2600
1. Crie um repositório GitHub para seu código-fonte (não envie arquivos do jogo ou chaves). Extraia o ZIP e faça commit de todo o conteúdo, inclusive a pasta oculta `.github`, conservando a estrutura. Pode usar GitHub Desktop ou Git na linha de comando. O arquivo do workflow precisa ficar na branch padrão do repositório.
2. Em Actions, habilite os workflows se necessário. Escolha **Build Legacy Windows** > **Run workflow**, mantendo a versão `0.11.0-legacy`.
3. Se o build funcionar, baixe o artifact ZIP da execução. Se falhar, preserve o log do primeiro passo com erro. O workflow depende da disponibilidade dos SDKs e downloads do projeto original. Não é preciso habilitar acesso a segredos nem fornecer chaves do Switch.
4. Extraia a compilação em outra pasta, separada da instalação original. Preserve o jogo preparado e seus saves. Não substitua seus executáveis existentes até testar.
5. A DLL `acnh_game.dll` precisa ser gerada novamente pelo **acnh_prepare.exe** Legacy ou pelo launcher Legacy a partir de ExeFS/RomFS de uma cópia própria. Não reutilize a DLL antiga e não use o Importer original para essa nova preparação. Use uma pasta de saída nova e espaço livre suficiente. Para uma primeira execução, deixe DLCs opcionais de fora. A documentação do preparador está em `prepare/README.md`.
6. Teste sem Intel SDE. Se ocorrer erro de instrução ilegal (0xc000001d), informe o módulo e o log, pois pode restar uma instrução incompatível em uma dependência. Não desative o antivírus.

## Exemplo de preparação (somente depois de compilar)
No CMD aberto na pasta da distribuição Legacy:

```
acnh_prepare.exe --exefs "D:\jogo animal crossing\exefs" --romfs "D:\jogo animal crossing\romfs" --out "D:\ACNH-Legacy" --platforms this --jobs 2
```

Os caminhos são ilustrativos; confirme a presença de ambas as pastas. Não execute se a origem contiver apenas arquivos empacotados sem essas pastas. A preparação pode levar horas, exigirá espaço adicional e não foi testada neste ambiente. O parâmetro --jobs 2 é uma escolha conservadora para limitar o uso de RAM, não um requisito do projeto.
